#include "clara.hpp"
#include <fstream>
#include <iostream>
#include <system.io/system.io.Path.h>
#include <system.io/system.io.directory.h>
#include <system.io/system.io.fileinfo.h>
#include <system.net/system.net.http.httplistener.h>
#include <system.net/system.net.http.httplistenerexception.h>
#include <system.net/system.net.http.httplistenerresponse.h>
#include <thread>

void DefaultResponse(System::Net::Http::HttpListenerResponse *response, std::string const &text);
std::string LocalHost(unsigned int port);

int main(int argc, char *argv[])
{
    unsigned int port = 8888;
    bool showHelp = false;

    auto cli = clara::Opt(port, "port")
                   ["-p"]["--port"]("on what should I be listing? Default is 8888") |
               clara::Help(showHelp);

    auto result = cli.parse(clara::Args(argc, argv));
    if (!result)
    {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;

        cli.writeToStream(std::cout);

        return 1;
    }

    if (showHelp)
    {
        cli.writeToStream(std::cout);

        return 0;
    }

    System::Net::Http::HttpListener listener;

    listener.Prefixes()
        .push_back(LocalHost(port));

    try
    {
        listener.Start();

        while (true)
        {
            auto context = listener.GetContext();

            std::thread([context]() {
                if (context->Request()->RawUrl().size() == 0)
                {
                    DefaultResponse(context->Response(), "Empty");
                    return;
                }

                auto requestedFile = System::IO::FileInfo(
                            System::IO::Path::Combine(
                                System::IO::Directory::GetCurrentWorkingDirectory(),
                                context->Request()->RawUrl().substr(1))
                            );
                if (!requestedFile.Exists())
                {
                    DefaultResponse(context->Response(), "File does not exist");
                    return;
                }

                // This will only correctly server text files
                std::ifstream t(requestedFile.FullName());
                std::string str((std::istreambuf_iterator<char>(t)),
                                std::istreambuf_iterator<char>());

                // todo : determine content type and set the corresponding header

                context->Response()->WriteOutput(str);
                context->Response()->CloseOutput();

                delete context;
            })
                .detach();
        }

        listener.Stop();
    } catch (System::Net::Http::HttpListenerException const *ex)
    {
        std::cout << "Exception in http listener: " << ex->Message() << "\n";
    }

    return 0;
}

void DefaultResponse(System::Net::Http::HttpListenerResponse *response, std::string const &text)
{
    response->WriteOutput("<html><body><h1>" + text + "</h1></body</html>");
    response->CloseOutput();
}

std::string LocalHost(unsigned int port)
{
    std::stringstream ss;

    ss << "http://localhost:" << port << "/";

    return ss.str();
}
