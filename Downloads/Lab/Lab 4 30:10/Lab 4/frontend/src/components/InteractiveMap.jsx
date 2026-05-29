import React, { useState } from 'react';
import { MapContainer, TileLayer, Marker, Popup, useMapEvents } from 'react-leaflet';
import L from 'leaflet';
import 'leaflet/dist/leaflet.css';

delete L.Icon.Default.prototype._getIconUrl;
L.Icon.Default.mergeOptions({
  iconRetinaUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon-2x.png',
  iconUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon.png',
  shadowUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-shadow.png',
});

function AddMarker({ onAdd }) {
  useMapEvents({
    click(e) {
      onAdd(e.latlng);
    },
  });
  return null;
}

export default function InteractiveMap() {
  const [markers, setMarkers] = useState([]);

  const addMarker = (latlng) => {
    setMarkers(prev => [...prev, {
      id: Date.now(),
      position: [latlng.lat, latlng.lng],
      name: `Marker ${prev.length + 1}`
    }]);
  };

  return (
    <div>
      <h2>3. Interactive Map (Click to Add)</h2>
      <div className="map-container">
        <MapContainer center={[51.505, -0.09]} zoom={13}>
          <TileLayer url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png" />
          {markers.map(m => (
            <Marker key={m.id} position={m.position}>
              <Popup>{m.name}</Popup>
            </Marker>
          ))}
          <AddMarker onAdd={addMarker} />
        </MapContainer>
      </div>
      <p style={{ marginTop: '8px', color: '#7f8c8d', fontSize: '14px' }}>
        Click on the map to add a marker.
      </p>
    </div>
  );
}