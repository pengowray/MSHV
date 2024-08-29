import http.server
import socketserver
import os

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

if __name__ == "__main__":
    PORT = 8000
    with socketserver.TCPServer(("", PORT), CustomHTTPRequestHandler) as httpd:
        print(f"Serving at port {PORT}")
        httpd.serve_forever()
