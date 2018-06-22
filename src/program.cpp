#include "clara.hpp"
#include <fstream>
#include <iostream>
#include <system.io/Directory.h>
#include <system.io/FileInfo.h>
#include <system.io/Path.h>
#include <system.net/HttpListener.h>
#include <system.net/HttpListenerException.h>
#include <system.net/HttpListenerResponse.h>
#include <thread>

void defaultResponse(System::Net::Http::HttpListenerResponse *response, std::string const &text);

int main(int argc, char *argv[])
{
    unsigned int port = 8888;
    std::string path = System::IO::Directory::GetCurrentWorkingDirectory();
    bool showHelp = false;

    auto cli = clara::Opt(port, "port")
                   ["-p"]["--port"]("on what should I be listing? Default is 8888") |
               clara::Arg(path, "path")("which path to serve static files from? Default is current working directory") |
               clara::Help(showHelp);

    auto result = cli.parse(clara::Args(argc, argv));
    if (!result)
    {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        exit(1);
    }

    if (showHelp)
    {
        cli.writeToStream(std::cout);
        exit(0);
    }

    path = System::IO::Directory::GetCurrentWorkingDirectory();
    std::cout << System::IO::Directory::GetCurrentWorkingDirectory() << std::endl;

    System::Net::Http::HttpListener listener;

    std::stringstream ss;
    ss << "http://localhost:" << port << "/";
    listener.Prefixes().push_back(ss.str());

    try
    {
        listener.Start();

        while (true)
        {
            auto context = listener.GetContext();

            std::thread t([context, path]() {
                if (context->Request()->RawUrl().size() == 0)
                {
                    defaultResponse(context->Response(), "Empty");
                }

                auto requestedFile = System::IO::FileInfo(System::IO::Path::Combine(path, context->Request()->RawUrl().substr(1)));
                if (!requestedFile.Exists())
                {
                    defaultResponse(context->Response(), "File does not exist");
                }

                // This will only correctly server text files
                std::ifstream t(requestedFile.FullName());
                std::string str((std::istreambuf_iterator<char>(t)),
                                std::istreambuf_iterator<char>());

                // todo : determine content type and set the corresponding header

                context->Response()->WriteOutput(str);
                context->Response()->CloseOutput();

                delete context;
            });

            t.detach();
        }

        listener.Stop();
    } catch (System::Net::Http::HttpListenerException const *ex)
    {
        std::cout << "Exception in http listener: " << ex->Message() << "\n";
    }

    return 0;
}

void defaultResponse(System::Net::Http::HttpListenerResponse *response, std::string const &text)
{
    response->WriteOutput("<html><body><h1>" + text + "</h1></body</html>");
    response->CloseOutput();
}
