#include "crow.h"
#include <httplib.h>
#include <chrono>

namespace {
crow::response forwardGetRequest(const std::string& cpp_app_url,
                                    const char* backend_path,const char* request_name,
                                    const char* server_name)
    {
        httplib::Client client(cpp_app_url);
        client.set_connection_timeout(std::chrono::seconds(1));
        client.set_read_timeout(std::chrono::seconds(1));
        client.set_write_timeout(std::chrono::seconds(3));


        auto result = client.Get(backend_path);

        if (!result) {
            return crow::response(
                502,
                std::string(server_name) + request_name + "  request failed: " +
                    httplib::to_string(result.error()));
        }

        CROW_LOG_INFO << request_name << "response: " << result->body;

        crow::response response(result->status,result->body);

        const auto content_type = result->get_header_value("Content-Type");
        
        if(!content_type.empty()) {
            response.set_header("Content-Type",content_type);
        }

        return response;
    }

}


int  main()
{
    crow::SimpleApp app;
    const char* value = std::getenv("CPP_APP_URL");
    const std::string cpp_app_url = value ? value : "http://127.0.0.1:9091";

    CROW_ROUTE(app,"/app/v1/execute")([cpp_app_url](){
        return forwardGetRequest(cpp_app_url,"/app/v1/healthz", "info","cpp-app");

    });

    CROW_ROUTE(app,"/app/v1/getinfo")([cpp_app_url](){
       return forwardGetRequest(cpp_app_url,"/app/v1/info", "info","cpp-app");

    });

    app.port(9092).multithreaded().run();
};
