#pragma once

#include <string>
#include <vector>
#include <memory>

#include <git2/repository.h>
#include <git2/remote.h>

#include <showlib/JSONSerializable.h>
#include <nlohmann/json.hpp>
#include <git/Git.h>

/**
 * SSH Credentials we might use. We may or may not actually store these,
 * but we cache them so we don't have to keep asking.
 *
 * Note that we don't currently support SSH keys with a blank password.
 * If a user does this, we might end up repeatedly asking for the password.
 */
class SSHKey: public ShowLib::JSONSerializable {
public:
    typedef std::shared_ptr<SSHKey> Pointer;
    typedef ShowLib::JSONSerializableVector<SSHKey> Vector;

    SSHKey() = default;
    virtual ~SSHKey();

    SSHKey & setFile(const std::string &);
    SSHKey & setHost(const std::string &);
    SSHKey & setPassword(const std::string &);
    SSHKey & setSavePassword(bool);

    const std::string & getPublicFile() const { return publicFile; }
    const std::string & getPrivateFile() const { return privateFile; }
    const std::string & getHost() const { return host; }
    const std::string & getPassword() const { return password; }

    void fromJSON(const JSON &) override;
    JSON & toJSON(JSON &) const override;

private:
    std::string		publicFile;
    std::string		privateFile;
    std::string		host;
    std::string		password;
    bool			savePassword = false;
};

/**
 * This represents a single repository we're monitoring.
 */
class Repository : public ShowLib::JSONSerializable {
public:
    typedef std::shared_ptr<Repository> Pointer;
    typedef ShowLib::JSONSerializableVector<Repository> Vector;

    Repository() = default;
    virtual ~Repository();

    std::string getDirectory() const { return directory; }
    bool isHidden() const { return hidden; }

    Repository & setDirectory(const std::string &value);
    Repository & setIsHidden(bool value) { hidden = value; return *this; }
    Repository & setKey(SSHKey::Pointer &value) { sshKey = value; return *this; }

    void fromJSON(const JSON &) override;
    JSON & toJSON(JSON &) const override;

    //--------------------------------------------------
    // These methods involve the git2 library.
    //--------------------------------------------------
    std::string currentBranch();

    Git::Repository::Pointer gitRepository() { return repository; }
    Git::Remote::Pointer gitRemote() { return remote; }

private:
    std::string	directory;
    std::string branchName;
    bool hidden = false;

    SSHKey::Pointer sshKey = nullptr;

    Git::Repository::Pointer repository = nullptr;
    Git::Remote::Pointer remote = nullptr;
};

/**
 * Our configuration. Some of this gets stored to a config file,
 * and some is looked up at runtime.
 */
class Configuration: public ShowLib::JSONSerializable
{
private:
    static Configuration * s_singleton;

    /** Our known SSH credentials. */
    SSHKey::Vector credentials;

    /** These are the top directories that each represent a repository we're monitoring. */
    Repository::Vector  repositories;

    /** Organize the GUI. Set to 0 for organization based on columns. */
    int rows = 0;

    /** Organize the GUI. Set to 0 for organization based on rows. */
    int columns = 0;

    std::string homeDirectory;

    // Private constructor to enforce the singleton.
    Configuration() = default;
    static std::string getConfigurationDirectory();

    SSHKey::Pointer lookupCredentialForHost(const std::string &host);

public:
    static Configuration & singleton();

    /** Load the configuration file. Returns 0 on success, -1 if not found. */
    static int load();
    static void save();

    void fromJSON(const JSON &) override;
    JSON & toJSON(JSON &) const override;

    Repository::Pointer addRepository(const std::string &);
    int getRows() const { return rows; }
    int getColumns() const { return columns; }
    const Repository::Vector & getRepositories() const { return repositories; };
};

