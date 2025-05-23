
![image](https://github.com/user-attachments/assets/114be6b4-82e0-4959-9334-ef7bfd16dc99)
The Mumbai Local Train Route Planner is a web-based application built using the Flask framework in
Python for the backend, with HTML, CSS, and JavaScript for the frontend. The architecture follows a
client-server model:
- Backend (Flask): Handles API requests, processes station data, and computes routes using
Dijkstra’s algorithm. It serves static files (map.js, style.css) and dynamic templates (index.html,
map.html).
- Frontend: Utilizes Bootstrap for responsive design, Leaflet.js for interactive map visualization, and
custom JavaScript for real-time user interaction. The map displays stations on Mumbai’s Western,
Central, and Harbor lines, with crowd level indicators.
- Data Storage: Station and connection data are stored in map.js as JSON-like structures, loaded
dynamically by the backend.
- External Libraries:
- Leaflet.js for map rendering.
- Bootstrap for UI components.
- OpenStreetMap for map tiles.
The application supports route planning with an option to avoid crowded stations, updating crowd
levels dynamically through a simulation endpoint.

![image](https://github.com/user-attachments/assets/b06a15ab-125a-4504-a36e-0a727aff1521)
