
#include "HttpServer.h"
#include "hssl.h"

int main() {
	HV_MEMCHECK;

	HttpService router;
	router.GET("/ping", [](HttpRequest* req, HttpResponse* resp) {
			return resp->String("pong");
			});

	router.GET("/data", [](HttpRequest* req, HttpResponse* resp) {
			static char data[] = "0123456789";
			return resp->Data(data, 10);
			});

	router.GET("/paths", [&router](HttpRequest* req, HttpResponse* resp) {
			return resp->Json(router.Paths());
			});

	router.POST("/echo", [](HttpRequest* req, HttpResponse* resp) {
			resp->content_type = req->content_type;
			resp->body = req->body;
			return 200;
			});

	router.GET("/", [](HttpRequest* req, HttpResponse* resp) {
			resp->body = "<html><head><title>Uniqs Exporter</title></head>"
			"<body><h1>Uniqs Exporter</h1><p><a href=\"/metrics\">Metrics</a></p></body></html>";
			return 200;
			});
	router.GET("/metrics", [](HttpRequest* req, HttpResponse* resp) {
			resp->body = ""
			"# HELP node_cpu_seconds_total Seconds the CPUs spent in each mode.\n"
			"# TYPE node_cpu_seconds_total counter\n"
			"node_cpu_seconds_total{cpu=\"0\",mode=\"idle\"} 1.19745322e+06\n"
			"node_cpu_seconds_total{cpu=\"0\",mode=\"iowait\"} 23.87\n"
			"node_cpu_seconds_total{cpu=\"0\",mode=\"irq\"} 1454.92\n"
			"node_cpu_seconds_total{cpu=\"0\",mode=\"nice\"} 30.91\n"
			"node_cpu_seconds_total{cpu=\"0\",mode=\"softirq\"} 996.35\n"
			"node_cpu_seconds_total{cpu=\"0\",mode=\"steal\"} 0\n"
			"node_cpu_seconds_total{cpu=\"0\",mode=\"system\"} 5871.98\n"
			"node_cpu_seconds_total{cpu=\"0\",mode=\"user\"} 6817.75\n"
			"node_cpu_seconds_total{cpu=\"1\",mode=\"idle\"} 1.19493468e+06\n"
			"node_cpu_seconds_total{cpu=\"1\",mode=\"iowait\"} 14.25\n"
			"node_cpu_seconds_total{cpu=\"1\",mode=\"irq\"} 2729.72\n"
			"node_cpu_seconds_total{cpu=\"1\",mode=\"nice\"} 30.8\n"
			"node_cpu_seconds_total{cpu=\"1\",mode=\"softirq\"} 2545.56\n"
			"node_cpu_seconds_total{cpu=\"1\",mode=\"steal\"} 0\n"
			"node_cpu_seconds_total{cpu=\"1\",mode=\"system\"} 5419.77\n"
			"node_cpu_seconds_total{cpu=\"1\",mode=\"user\"} 6972.07\n"
			"node_cpu_seconds_total{cpu=\"2\",mode=\"idle\"} 1.19809115e+06\n"
			"node_cpu_seconds_total{cpu=\"2\",mode=\"iowait\"} 21.33\n"
			"node_cpu_seconds_total{cpu=\"2\",mode=\"irq\"} 1453.61\n"
			"node_cpu_seconds_total{cpu=\"2\",mode=\"nice\"} 34.88\n"
			"node_cpu_seconds_total{cpu=\"2\",mode=\"softirq\"} 967.57\n"
			"node_cpu_seconds_total{cpu=\"2\",mode=\"steal\"} 0\n"
			"node_cpu_seconds_total{cpu=\"2\",mode=\"system\"} 5629.16\n"
			"node_cpu_seconds_total{cpu=\"2\",mode=\"user\"} 6891.54\n"
			"node_cpu_seconds_total{cpu=\"3\",mode=\"idle\"} 1.19795439e+06\n"
			"node_cpu_seconds_total{cpu=\"3\",mode=\"iowait\"} 17.25\n"
			"node_cpu_seconds_total{cpu=\"3\",mode=\"irq\"} 1486.87\n"
			"node_cpu_seconds_total{cpu=\"3\",mode=\"nice\"} 40.87\n"
			"node_cpu_seconds_total{cpu=\"3\",mode=\"softirq\"} 1094.84\n"
			"node_cpu_seconds_total{cpu=\"3\",mode=\"steal\"} 0\n"
			"node_cpu_seconds_total{cpu=\"3\",mode=\"system\"} 5650.75\n"
			"node_cpu_seconds_total{cpu=\"3\",mode=\"user\"} 6880.25\n"
			;
			return 200;
			});

	http_server_t server;
	server.service = &router;
	server.port = 8080;
#if 0
	server.https_port = 8443;
	hssl_ctx_init_param_t param;
	memset(&param, 0, sizeof(param));
	param.crt_file = "cert/server.crt";
	param.key_file = "cert/server.key";
	if (hssl_ctx_init(&param) == NULL) {
		fprintf(stderr, "SSL certificate verify failed!\n");
		return -20;
	}
#endif

	//uncomment to test multi-processes
	// server.worker_processes = 4;
	// uncomment to test multi-threads
	// server.worker_threads = 4;

#if 1
	http_server_run(&server);
#else
	// test http_server_stop
	http_server_run(&server, 0);
	sleep(10);
	http_server_stop(&server);
#endif
	return 0;
}
