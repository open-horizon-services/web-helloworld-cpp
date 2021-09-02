#include <thread>
#include <memory>
#include <string.h>
#include <cstdlib>
#include <restbed>

using namespace std;
using namespace restbed;

const char* page = R"HEREDOC(
<!DOCTYPE html>
<html><head>
<meta charset=\"utf-8\">
<title>WebHello</title>
</head><body>Hello, "%s".</body></html
)HEREDOC";

void handle_get(const shared_ptr<Session> session) {
  auto addr = session->get_origin();
  std::string host = addr.substr(1, addr.find("]") - 1);
  if (!host.substr(0, 7).compare("::ffff:"))
    host = host.substr(7);
  char content[BUFSIZ];
  (void) snprintf(content, BUFSIZ, page, host.c_str());
  char len[10];
  (void) snprintf(len, 10, "%u", (unsigned)strlen(content));
  session->close(OK, content, {{"Content-Length",len},{"Connection","close"}});
}

int main(const int, const char**) {
  auto resource = make_shared<Resource>();
  resource->set_path("/");
  resource->set_method_handler("GET", handle_get);
  auto settings = make_shared<Settings>();
  settings->set_port(8000);
  auto service = make_shared<Service>();
  service->publish(resource);
  service->start(settings);
  return EXIT_SUCCESS;
}

