/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "JuceHeader.h"

namespace element {

//==============================================================================
/**
    Acts as the slave end of a master/slave pair of connected processes.

    The ChildProcessSlave and ChildProcessMaster classes make it easy for an app
    to spawn a child process, and to manage a 2-way messaging connection to control it.

    To use the system, you need to create subclasses of both ChildProcessSlave and
    ChildProcessMaster. To instantiate the ChildProcessSlave object, you must
    add some code to your main() or JUCEApplication::initialise() function that
    calls the initialiseFromCommandLine() method to check the app's command-line
    parameters to see whether it's being launched as a child process. If this returns
    true then the slave process can be allowed to run, and its handleMessageFromMaster()
    method will be called whenever a message arrives.

    The juce demo app has a good example of this class in action.

    @see ChildProcessMaster, InterprocessConnection, ChildProcess
*/
class JUCE_API ChildProcessSlave
{
public:
    /** Creates a non-connected slave process.
        Use initialiseFromCommandLine to connect to a master process.
    */
    ChildProcessSlave();

    /** Destructor. */
    virtual ~ChildProcessSlave();

    /** This checks some command-line parameters to see whether they were generated by
        ChildProcessMaster::launchSlaveProcess(), and if so, connects to that master process.

        In an exe that can be used as a child process, you should add some code to your
        main() or JUCEApplication::initialise() that calls this method.

        The commandLineUniqueID should be a short alphanumeric identifier (no spaces!)
        that matches the string passed to ChildProcessMaster::launchSlaveProcess().

        The timeoutMs parameter lets you specify how long the child process is allowed
        to run without receiving a ping from the master before the master is considered to
        have died, and handleConnectionLost() will be called. Passing <= 0 for this timeout
        makes it use a default value.

        Returns true if the command-line matches and the connection is made successfully.
    */
    bool initialiseFromCommandLine (const juce::String& commandLine,
                                    const juce::String& commandLineUniqueID,
                                    int timeoutMs = 0);

    //==============================================================================
    /** This will be called to deliver messages from the master process.
        The call will probably be made on a background thread, so be careful with your
        thread-safety! You may want to respond by sending back a message with
        sendMessageToMaster()
    */
    virtual void handleMessageFromMaster (const juce::MemoryBlock&) = 0;

    /** This will be called when the master process finishes connecting to this slave.
        The call will probably be made on a background thread, so be careful with your thread-safety!
    */
    virtual void handleConnectionMade();

    /** This will be called when the connection to the master process is lost.
        The call may be made from any thread (including the message thread).
        Typically, if your process only exists to act as a slave, you should probably exit
        when this happens.
    */
    virtual void handleConnectionLost();

    /** Tries to send a message to the master process.
        This returns true if the message was sent, but doesn't check that it actually gets
        delivered at the other end. If successful, the data will emerge in a call to your
        ChildProcessMaster::handleMessageFromSlave().
    */
    bool sendMessageToMaster (const juce::MemoryBlock&);

private:
    struct Connection;
    friend struct Connection;
    std::unique_ptr<Connection> connection;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChildProcessSlave)
};

//==============================================================================
/**
    Acts as the master in a master/slave pair of connected processes.

    The ChildProcessSlave and ChildProcessMaster classes make it easy for an app
    to spawn a child process, and to manage a 2-way messaging connection to control it.

    To use the system, you need to create subclasses of both ChildProcessSlave and
    ChildProcessMaster. When you want your master process to launch the slave, you
    just call launchSlaveProcess(), and it'll attempt to launch the executable that
    you specify (which may be the same exe), and assuming it has been set-up to
    correctly parse the command-line parameters (see ChildProcessSlave) then a
    two-way connection will be created.

    The juce demo app has a good example of this class in action.

    @see ChildProcessSlave, InterprocessConnection, ChildProcess
*/
class JUCE_API ChildProcessMaster
{
public:
    /** Creates an uninitialised master process object.
        Use launchSlaveProcess to launch and connect to a child process.
    */
    ChildProcessMaster();

    /** Destructor. */
    virtual ~ChildProcessMaster();

    /** Attempts to launch and connect to a slave process.
        This will start the given executable, passing it a special command-line
        parameter based around the commandLineUniqueID string, which must be a
        short alphanumeric string (no spaces!) that identifies your app. The exe
        that gets launched must respond by calling ChildProcessSlave::initialiseFromCommandLine()
        in its startup code, and must use a matching ID to commandLineUniqueID.

        The timeoutMs parameter lets you specify how long the child process is allowed
        to go without sending a ping before it is considered to have died and
        handleConnectionLost() will be called. Passing <= 0 for this timeout makes
        it use a default value.

        If this all works, the method returns true, and you can begin sending and
        receiving messages with the slave process.
    */
    bool launchSlaveProcess (const juce::File& executableToLaunch,
                             const juce::String& commandLineUniqueID,
                             int timeoutMs = 0,
                             int streamFlags = juce::ChildProcess::wantStdOut | juce::ChildProcess::wantStdErr);

    /** This will be called to deliver a message from the slave process.
        The call will probably be made on a background thread, so be careful with your thread-safety!
    */
    virtual void handleMessageFromSlave (const juce::MemoryBlock&) = 0;

    /** This will be called when the slave process dies or is somehow disconnected.
        The call will probably be made on a background thread, so be careful with your thread-safety!
    */
    virtual void handleConnectionLost();

    /** Attempts to send a message to the slave process.
        This returns true if the message was dispatched, but doesn't check that it actually
        gets delivered at the other end. If successful, the data will emerge in a call to
        your ChildProcessSlave::handleMessageFromMaster().
    */
    bool sendMessageToSlave (const juce::MemoryBlock&);

private:
    juce::ChildProcess childProcess;

    struct Connection;
    friend struct Connection;
    std::unique_ptr<Connection> connection;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChildProcessMaster)
};

} // namespace element
