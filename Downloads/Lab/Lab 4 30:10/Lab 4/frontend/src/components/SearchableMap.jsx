import React, { useState } from 'react';
import { MapContainer, TileLayer, Marker, Popup, useMap } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';
import L from 'leaflet';

delete L.Icon.Default.prototype._getIconUrl;
L.Icon.Default.mergeOptions({
  iconRetinaUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon-2x.png',
  iconUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon.png',
  shadowUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-shadow.png',
});

function ChangeView({ center, zoom }) {
  const map = useMap();
  map.setView(center, zoom);
  return null;
}

const cities = [
  { name: 'London', position: [51.505, -0.09] },
  { name: 'Paris', position: [48.8566, 2.3522] },
  { name: 'New York', position: [40.7128, -74.0060] },
  { name: 'Tokyo', position: [35.6762, 139.6503] },
  { name: 'Sydney', position: [-33.8688, 151.2093] },
];

export default function SearchableMap() {
  const [selectedCity, setSelectedCity] = useState(cities[0]);

  return (
    <div>
      <h2>4. Searchable Map</h2>
      <div className="map-container">
        <div className="select-overlay">
          <select
            value={selectedCity.name}
            onChange={(e) => setSelectedCity(cities.find(c => c.name === e.target.value))}
          >
            {cities.map(city => (
              <option key={city.name} value={city.name}>{city.name}</option>
            ))}
          </select>
        </div>

        <MapContainer center={selectedCity.position} zoom={13}>
          <TileLayer
            attribution='&copy; OpenStreetMap contributors'
            url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
          />
          <Marker position={selectedCity.position}>
            <Popup>{selectedCity.name}</Popup>
          </Marker>
          <ChangeView center={selectedCity.position} zoom={13} />
        </MapContainer>
      </div>
    </div>
  );
}