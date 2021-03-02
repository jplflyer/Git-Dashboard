#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

#include <git2.h>
#include <nlohmann/json.hpp>
#include <git/Git.h>
#include <showlib/SSHConfiguration.h>

#include "Configuration.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using namespace ShowLib;

Configuration * Configuration::s_singleton = nullptr;

/**
 * Retrieve our singleton.
 */
Configuration &
Configuration::singleton() {
    if (s_singleton == nullptr) {
        s_singleton = new Configuration();
    }

    return *s_singleton;
}

/**
 * Load the configuration file.
 */
int
Configuration::load() {
    string dirName = getConfigurationDirectory();
    string fileName = dirName + "/config.json";
    int retVal = 1;

    if (std::filesystem::exists(fileName) && std::filesystem::is_regular_file(fileName)) {
        std::ifstream configFile(fileName);
        std::stringstream buffer;
        buffer << configFile.rdbuf();
        string contents = buffer.str();

        if (contents.size() > 0u && contents.at(0) == '{') {
            JSON json = JSON::parse(buffer.str());
            singleton().fromJSON(json);
            retVal = 0;
        }
    }

    return retVal;
}

/**
 * Save the configuration.
 */
void
Configuration::save() {
    string dirName = getConfigurationDirectory();
    string fileName = dirName + "/config.json";

    JSON json;
    singleton().toJSON(json);

    if (!std::filesystem::exists(dirName)) {
        cout << "Create directories: " << dirName << endl;
        std::filesystem::create_directories(dirName);
    }
    cout << "Create file: " << fileName << endl;
    std::ofstream ofs(fileName);
    ofs << json.dump(2) << endl;
}

/**
 * Return our path to our configuration directory.
 * This might need some if-defing under Windows.
 */
string
Configuration::getConfigurationDirectory() {
    struct passwd* pwd = getpwuid(getuid());
    string retVal = string(pwd->pw_dir) + "/Library/Application Support/Git-Dashboard";

    return retVal;
}

/**
 * Populate ourself from this JSON.
 */
void
Configuration::fromJSON(const JSON &json) {
    JSON repos = jsonArray(json, "repositories");
    rows = intValue(json, "rows");
    columns = intValue(json, "columns");
    repositories.fromJSON(repos);
}

/**
 * Write ourself to this JSON.
 */
JSON &
Configuration::toJSON(JSON &json) const {
    JSON repos = JSON::array();

    repositories.toJSON(repos);

    json["rows"] = rows;
    json["columns"] = columns;
    json["repositories"] = repos;

    return json;
}

/**
 * Add to the repository, returning true if we added one, false if we already had it.
 */
Repository::Pointer
Configuration::addRepository(const std::string &dirName) {
    Repository::Pointer existing = repositories.findIf(
                [=](Repository::Pointer ptr){ return ptr->getDirectory() == dirName; } );

    if (existing != nullptr) {
        return existing;
    }

    Repository::Pointer ptr = std::make_shared<Repository>();
    ptr->setDirectory(dirName);
    repositories.push_back(ptr);

    //----------------------------------------------------------------------
    // Make sure we have the SSH Key Cached.
    //----------------------------------------------------------------------
    Git::URI uri { ptr->gitRemote()->url() };
    SSHKey::Pointer sshKey = lookupCredentialForHost(uri.getHost());
    ptr->setKey(sshKey);

    return ptr;
}

/**
 * This does a lookup of the credentials for this host. We may already have
 * it cached. If not, we add it.
 */
SSHKey::Pointer
Configuration::lookupCredentialForHost(const std::string &hostStr) {
    //--------------------------------------------------
    // See if we have this host. This is the simple case.
    //--------------------------------------------------
    for (SSHKey::Pointer &key: credentials) {
        if (key->getHost() == hostStr) {
            return key;
        }
    }

    //--------------------------------------------------
    // First we need to get the identity file to use.
    //--------------------------------------------------
    ShowLib::SSH::Configuration & config = ShowLib::SSH::Configuration::singleton();

    if (!config.isLoaded()) {
        config.load();
    }

    ShowLib::SSH::Host::Pointer host = config.getHost(hostStr);
    if (host == nullptr) {
        cerr << "Can't find SSH Key to use for this repo." << endl;
        return nullptr;
    }

    string identityFile = host->getIdentityFile();

    //--------------------------------------------------
    // Then we have to see if we already have a key
    // for the same identity file but a different host.
    //--------------------------------------------------
    for (SSHKey::Pointer &key: credentials) {
        if (key->getPrivateFile() == identityFile) {
            return key;
        }
    }

    //--------------------------------------------------
    // We need to create a new one.
    //--------------------------------------------------
    SSHKey::Pointer newHost = std::make_shared<SSHKey>();
    newHost->setHost(hostStr);
    newHost->setFile(identityFile);
    credentials.push_back(newHost);

    return newHost;
}



//======================================================================
// Repositories.
//======================================================================

/**
 * Destructor.
 */
Repository::~Repository() {
}

void Repository::fromJSON(const JSON &json) {
    setDirectory(stringValue(json, "directory"));
    setIsHidden(boolValue(json, "hidden"));
}

JSON & Repository::toJSON(JSON &json) const {
    json["directory"] = directory;
    json["hidden"] = hidden;

    return json;
}

/**
 * Set our directory.
 */
Repository &
Repository::setDirectory(const std::string &value) {
    directory = value;
    repository = std::make_shared<Git::Repository>(directory);
    remote = repository->getRemote();

    return *this;
}

/**
 * What branch are we on?
 */
string
Repository::currentBranch() {
    if (branchName.length() == 0) {
    }

    return branchName;
}

//======================================================================
// SSH Credential Info
//======================================================================

SSHKey::~SSHKey() {
}

/**
 * Files are named as "foo" for the private file and "foo.pub"
 * for the public file.
 */
SSHKey & SSHKey::setFile(const std::string &value) {
    privateFile = value;
    publicFile = privateFile + ".pub";
    return *this;
}

/**
 * What host is this key for?
 */
SSHKey & SSHKey::setHost(const std::string &value) {
    host = value;
    return *this;
}

/**
 * SSH Keys require a password (probably).
 */
SSHKey & SSHKey::setPassword(const std::string &value) {
    password = value;
    return *this;
}

/**
 * We will only store the credentials if the user specifically asks
 * us to.
 */
SSHKey & SSHKey::setSavePassword(bool value) {
    savePassword = value;
    return *this;
}

/**
 * Populate from JSON.
 */
void SSHKey::fromJSON(const JSON &json) {
    setFile(stringValue(json, "file"));
    setHost(stringValue(json, "host"));
    setPassword(stringValue(json, "password"));
    savePassword = boolValue(json, "savePassword");
}

/**
 * Write to JSON.
 */
JSON & SSHKey::toJSON(JSON &json) const {
    json["file"] = privateFile;
    json["host"] = host;
    if (savePassword) {
        json["password"] = password;
        json["savePassword"] = savePassword;
    }

    return json;
}
