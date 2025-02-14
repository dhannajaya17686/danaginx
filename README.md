# Custom HTTP Web Server

A basic custom HTTP web server built in C that supports dynamic route generation, serving static files (HTML, CSS, JS, JPEG, PNG, etc.), and automatically mapping routes based on the folder structure.

## Features

- **Automatic Route Generation**: The server automatically generates routes from the folder structure inside the `templates` directory.
- **File Type Support**: Supports `.html`, `.css`, `.js`, `.jpeg`, `.png` and other static file types, with appropriate headers.
- **Index Handling**: Automatically serves `index.html` as the root route (`/`).
- **404 Handling**: Returns a `404 Not Found` error for unmatched routes.

## How to Run

### 1. Clone the Repository

Clone the repository to your local machine:

```bash
git clone <repository-url>
```

### 2. Build the Server

Make sure you have a C compiler (like gcc) installed. Then, compile the server:

```bash
gcc -o danaginx  src/main.c src/request.c src/response.c src/routes.c src/server.c -Iinclude
```

This will create an executable called `http_server`.

### 3. Run the Server

After building the server, you can run it with the following command:

```bash
./http_server
```

By default, the server will start and listen for incoming requests on port 8080:

```
Server is running at http://localhost:8080
```

### 4. Access the Server

- Open your browser and go to [http://localhost:8080](http://localhost:8080).
- The server will automatically generate routes from the `templates` folder.
  - For example, a file `templates/index.html` will be accessible via `http://localhost:8080/`.
  - Similarly, files like `templates/about.html`, `templates/contact.html`, and images like `templates/images/logo.png` can be accessed through their respective routes, e.g., `http://localhost:8080/about` and `http://localhost:8080/images/logo.png`.

### 5. Stopping the Server

To stop the server, simply press `Ctrl+C` in the terminal where the server is running. This will terminate the server process and free up the port.

## Directory Structure

Example Structure:

```
templates/
  index.html
  about.html
  contact.html
  images/
    logo.png
  css/
    style.css
```

This structure will result in the following routes:

```
/ → templates/index.html
/about → templates/about.html
/contact → templates/contact.html
/images/logo.png → templates/images/logo.png
/css/style.css → templates/css/style.css
```

### Files and Folders:

- `templates/`: Contains HTML and static files. Routes are automatically generated based on the folder structure inside this directory.
- `static/`: Holds static assets such as `.css`, `.js`, and image files.
- `images/`: Stores image files like `.jpeg`, `.png`, etc.

## How Routing Works

- **Route Generation**: The server scans the `templates` folder and generates routes based on the directory structure. Each file and subfolder becomes a route.
  - Example: `templates/about.html` becomes the route `/about`.
- **Serving Static Files**: The server supports static file types like `.html`, `.css`, `.js`, `.jpeg`, `.png`, etc.
  - The correct MIME type is automatically set based on the file extension (e.g., `Content-Type: image/jpeg` for `.jpeg` files).
- **404 Handling**: If a requested route does not match an existing file, the server responds with a `404 Not Found` error.

## File Types and Headers

The server handles different types of files and adjusts the `Content-Type` header accordingly:

| File Type | Content-Type Header      |
| --------- | ------------------------ |
| HTML      | `text/html`              |
| CSS       | `text/css`               |
| JS        | `application/javascript` |
| JPEG      | `image/jpeg`             |
| PNG       | `image/png`              |

## Example Request-Response Flow

1. **Request**: A client sends an HTTP request to `GET /about`.
2. **Route Match**: The server checks if `/about` exists in the `templates` directory. If it matches `templates/about.html`, it serves the file.
3. **Response**: The server responds with the contents of `templates/about.html`, setting the appropriate headers.

## Serving Index Files

The `index.html` file located in any directory is automatically served as the root route. For example:

- `templates/index.html` is served at [http://localhost:8080/](http://localhost:8080/).

## Future Enhancements

- **Dynamic Content Handling**: The server currently serves static files, but could be extended to handle dynamic content (e.g., user authentication, data fetching).
- **Improved Error Handling**: Provide more detailed error messages and logging for debugging.
- **Caching**: Implement caching mechanisms to improve performance, especially for static files.
- **HTTPS Support**: Add support for HTTPS to secure the communication.
