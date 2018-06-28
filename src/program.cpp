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
std::string contentTypeFromExtension(std::string const &extension);

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
                        context->Request()->RawUrl().substr(1)));
                if (!requestedFile.Exists())
                {
                    DefaultResponse(context->Response(), "File does not exist");
                    return;
                }

                std::ifstream t(requestedFile.FullName(), std::ios::binary);
                std::vector<char> buffer((std::istreambuf_iterator<char>(t)),
                                         std::istreambuf_iterator<char>());

                context->Response()->AddHeader("Content-Type", contentTypeFromExtension(requestedFile.Extension()));

                context->Response()->WriteOutput(buffer);
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

std::string contentTypeFromExtension(std::string const &extension)
{
    if (extension == ".exe") return "application/octet-stream";
    if (extension == ".html") return "text/html";
    if (extension == ".htm") return "text/html";
    if (extension == ".css") return "text/css";
    if (extension == ".js") return "text/javascript";
    if (extension == ".jpg") return "image/jpe";
    if (extension == ".jpeg") return "image/jpeg";
    if (extension == ".gif") return "image/gif";
    if (extension == ".png") return "image/png";
    if (extension == ".svg") return "image/svg+xml";
    if (extension == ".ico") return "image/x-icon";

    return "text/plain";
}
