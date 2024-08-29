import http.server
import socketserver
import os
import argparse
import signal

class CustomHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    # Define the root directories
    base_path = os.path.abspath('web')  # Serve from 'web/' by default
    build_path = os.path.abspath('.')  # Serve '/build' from .

    def translate_path(self, path):
        # If the request path starts with /build, serve from the 'wasm/' directory
        if path.startswith('/build'):
            path = path.replace('/build', self.build_path, 1)
        else:
            # Serve all other paths from the 'web/' directory
            path = os.path.join(self.base_path, path.lstrip('/'))
        return path

def run_server(port):
    handler = CustomHTTPRequestHandler
    with socketserver.TCPServer(("", port), handler) as httpd:
        print(f"Serving at port {port}")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nShutting down the server...")
        finally:
            httpd.server_close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run a simple HTTP server")
    parser.add_argument('-p', '--port', type=int, default=8000, help="Port to run the server on (default: 8000)")
    args = parser.parse_args()

    run_server(args.port)