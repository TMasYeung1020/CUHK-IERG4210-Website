import React from 'react';
import { MapContainer, TileLayer, Marker, Popup } from 'react-leaflet';
import L from 'leaflet';
import 'leaflet/dist/leaflet.css';

delete L.Icon.Default.prototype._getIconUrl;
L.Icon.Default.mergeOptions({
  iconRetinaUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon-2x.png',
  iconUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon.png',
  shadowUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-shadow.png',
});

const createCustomIcon = (color) => {
  const colors = { red: '#e74c3c', blue: '#3498db', green: '#27ae60', default: '#2c3e50' };
  const svg = `
    <svg width="32" height="32" viewBox="0 0 32 32" xmlns="http://www.w3.org/2000/svg">
      <path d="M16 1 C10.5 1, 6 5.5, 6 11 C6 18, 16 31, 16 31 C16 31, 26 18, 26 11 C26 5.5, 21.5 1, 16 1 Z" 
            fill="${colors[color] || colors.default}" stroke="white" stroke-width="2"/>
      <circle cx="16" cy="11" r="4" fill="white"/>
    </svg>
  `;

  return L.divIcon({
    html: svg,
    iconSize: [32, 32],
    iconAnchor: [16, 32],
    popupAnchor: [0, -32],
    className: 'custom-marker',
  });
};

const locations = [
  { id: 1, position: [51.505, -0.09], name: 'London', color: 'red' },
  { id: 2, position: [51.51, -0.1], name: 'Location 2', color: 'blue' },
  { id: 3, position: [51.50, -0.08], name: 'Location 3', color: 'green' },
];

const MultiMarkerMap = () => {
  return (
    <div>
      <h2>2. Multi-Marker Map</h2>
      <div className="map-container">
        <MapContainer center={[51.505, -0.09]} zoom={13}>
          <TileLayer
            attribution='&copy; OpenStreetMap contributors'
            url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
          />
          {locations.map(loc => (
            <Marker key={loc.id} position={loc.position} icon={createCustomIcon(loc.color)}>
              <Popup>
                <strong>{loc.name}</strong><br />
                {loc.position[0].toFixed(4)}, {loc.position[1].toFixed(4)}
              </Popup>
            </Marker>
          ))}
        </MapContainer>
      </div>
    </div>
  );
};

export default MultiMarkerMap;