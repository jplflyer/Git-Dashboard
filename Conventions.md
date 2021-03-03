# Coding Conventions
Please don't reformat my code.

# Naming Conventions
I use Java naming conventions. Frankly, I think the standards committee missed when they stuck to all-lower-case on everything. I'm sure they had reasons, but I don't know what they are.

Class and Namespace names start with an upper case letter.

Fields and methods start with a lower case letter.

I use a combination of camelCase with underscores for readability. In other words, I prefer camelCase but will add underscores when names are exceedingly long and difficult to read.

Avoid very, very short variable names. `n` is not a good name.

Avoid names that clash with class names. std::vector is a class, so avoid vector as a name. This is a case where I'm willing to call it vec.

# Indentation and Spacing Style
One 4-space tab is preferred, but my IDE tends to insert 4 spaces instead of a tab. I use a vim mode and have this set:

    export EXINIT=":set ts=4 sw=4 ai showmode"

  Generally speaking, I prefer this:

    if (condition) {
    }
    else {
    }

If condition is exceptionally complicated or requires multiple lines, then I'll move the open brace to the next line.

In all cases, error on the side of readability. Add spaces as necessary so my old, old eyes can read the code. We have ample screen real estate. Don't squish it all together. If you 23 years old, maybe you can read that stream of text, but I need it broken up a bit.

# About Braces
Optional braces are not optional.

    if (condition) {
        return 0;
    }

Yes, I know that takes 3 lines when you could shove it all on one line. And sometimes (very rarely) I will. But treating optional braces as if they're optional is a VERY VERY common source of bugs.

# About Namespaces
I'm not very good at setting them. Consider this me slapping my own wrist. Feel free, if you want, to move things in my libraries into a reasonable (very very lightly nested) namespace if I haven't done it. Just warn me in the pull request.

I don't tend to use namespaces on apps. I don't feel it's necessary -- it's extra clutter. Happy to have a conversation about this some time. Great idea on libraries, unnecessary in apps, as it's not like you're going to collide anywhere else.

I'm willing to do:

    using namespace ShowLib;
    using std::cout;  // I do this all the time

But I stopped doing:

    using namespace std;

If you see this in my code, kill it.

# What's With the Pointers
You'll see that I subclassed std::vector as a JSONSerializableVector. Yeah, I know, I've since learned we're not supposed to do that, and I should wrap it instead. I'll probably do that someday. Feel free to do it for me if you're inspired. It won't need to implement all features of vector.

But that's my base class for most vectors, and it uses a std::shared_ptr<>. I shudder at the idea of inserting in the middle otherwise, but of course, I shudder worse at the idea of using raw pointers.
