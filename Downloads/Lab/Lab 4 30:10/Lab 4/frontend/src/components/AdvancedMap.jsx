// src/components/AdvancedMap.jsx
import React, { useState, useEffect } from 'react';
import { MapContainer, TileLayer, Marker, Popup, useMap, useMapEvents } from 'react-leaflet';
import L from 'leaflet';
import 'leaflet/dist/leaflet.css';

// === 修復圖示 ===
delete L.Icon.Default.prototype._getIconUrl;
L.Icon.Default.mergeOptions({
  iconRetinaUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon-2x.png',
  iconUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon.png',
  shadowUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-shadow.png',
});

const createShopIcon = () => {
  return L.icon({
    iconUrl: 'https://www.vhv.rs/dpng/d/581-5812950_mos-burger-hd-png-download.png',               // 真實 LOGO
    iconSize:     [24, 24],               // 圖片寬高
    iconAnchor:   [24, 48],               // 尖端對準經緯度
    popupAnchor:  [0, -48],               // Popup 位置
    className:    'mos-burger-marker'      // 自訂 CSS class
  });
};

// === 自訂圖示 ===
const createIcon = (type) => {
  const colors = {
    landmark: '#3498db',
    custom: '#9b59b6',
    mylocation: '#27ae60'
  };
  const svg = `
    <svg width="38" height="38" viewBox="0 0 38 38" xmlns="http://www.w3.org/2000/svg">
      <path d="M19 1 C11.5 1, 5 7.5, 5 15 C5 25, 19 37, 19 37 C19 37, 33 25, 33 15 C33 7.5, 26.5 1, 19 1 Z" 
            fill="${colors[type] || '#95a5a6'}" stroke="white" stroke-width="2.5"/>
      <circle cx="19" cy="14" r="5" fill="white"/>
    </svg>
  `;
  return L.divIcon({
    html: svg,
    iconSize: [38, 38],
    iconAnchor: [19, 38],
    popupAnchor: [0, -38],
    className: 'custom-marker',
  });
};

// === 學號邏輯 ===
const STUDENT_ID = '1155203181';
const LAST_DIGIT = parseInt(STUDENT_ID.slice(-1)); // 1
const DEFAULT_CENTER = { name: 'Pavilion of Harmony', position: [22.4188, 114.2085], address: 'Pavilion of Harmony' };

const MOS_BURGER_STORES = [
  { name: 'MOS Burger - Shatin', position: [22.3820, 114.1870], address: 'New Town Plaza' },
  { name: 'MOS Burger - Mong Kok', position: [22.3180, 114.1700], address: 'Argyle Street' },
  { name: 'MOS Burger - Tsim Sha Tsui', position: [22.2970, 114.1720], address: 'Nathan Road' },
];

// === 變更視圖 ===
function ChangeView({ center }) {
  const map = useMap();
  useEffect(() => {
    map.setView(center, 15, { animate: true });
  }, [center, map]);
  return null;
}

// === 點擊新增 ===
function ClickHandler({ onAdd }) {
  useMapEvents({
    click: (e) => onAdd(e.latlng),
  });
  return null;
}

// === 主元件 ===
export default function AdvancedMap() {
  const [center, setCenter] = useState(DEFAULT_CENTER.position);
  const [customMarkers, setCustomMarkers] = useState([]);

  const addCustomMarker = (latlng) => {
    setCustomMarkers(prev => [...prev, {
      id: Date.now(),
      position: [latlng.lat, latlng.lng],
      name: `Custom Marker ${prev.length + 1}`
    }]);
  };

  const clearCustomMarkers = () => setCustomMarkers([]);

  const goToMyLocation = () => {
    if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(
        pos => {
          const pos2d = [pos.coords.latitude, pos.coords.longitude];
          setCenter(pos2d);
          addCustomMarker({ lat: pos.coords.latitude, lng: pos.coords.longitude });
        },
        () => alert('無法取得位置，請允許定位')
      );
    } else {
      alert('瀏覽器不支援定位');
    }
  };

  return (
    <div className="advanced-map-wrapper">
      {/* 側邊欄 */}
      <div className="sidebar">
        <div className="info">
            <h1>Yeung Man (1155203181)</h1>
        </div>
        <div className="section">
          <h3>PREDEFINED LOCATIONS</h3>
          <button className="landmark-btn" onClick={() => setCenter(DEFAULT_CENTER.position)}>
            {DEFAULT_CENTER.name}
          </button>
        </div>

        <div className="section">
          <h3>MOS BURGER SHOPS</h3>
          {MOS_BURGER_STORES.map(s => (
            <button key={s.name} className="shop-btn" onClick={() => setCenter(s.position)}>
              {s.name}
            </button>
          ))}
        </div>

        <div className="section">
          <h3>ADD CUSTOM MARKERS</h3>
          <p className="info-text">Click anywhere on the map to add a marker</p>
          <p className="counter">Custom Markers: {customMarkers.length}</p>
          <button className="clear-btn" onClick={clearCustomMarkers}>
            Clear All Custom Markers
          </button>
        </div>

        <div className="section">
          <button className="mylocation-btn" onClick={goToMyLocation}>
            My Location
          </button>
        </div>

        <div className="section">
          <h3>YOUR CUSTOM MARKERS</h3>
          {customMarkers.length === 0 ? (
            <p className="no-markers">No custom markers added yet</p>
          ) : (
            <ul className="marker-list">
              {customMarkers.map(m => (
                <li key={m.id}>
                  <span className="marker-dot"></span>
                  {m.name}
                </li>
              ))}
            </ul>
          )}
        </div>
      </div>

      {/* 地圖 */}
      <div className="map-container">
        <MapContainer 
          center={center} 
          zoom={15} 
          className="leaflet-map"  // 關鍵！
        >
          <TileLayer
            url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
            attribution='&copy; OpenStreetMap contributors'
          />

          {/* 預設地標 */}
          <Marker position={DEFAULT_CENTER.position} icon={createIcon('landmark')}>
            <Popup>
              <strong>{DEFAULT_CENTER.name}</strong><br />
              {DEFAULT_CENTER.position}
              Default starting location (Student ID last digit: {LAST_DIGIT})
            </Popup>
          </Marker>

          {/* MOS Burger */}
          {MOS_BURGER_STORES.map(s => (
            <Marker key={s.name} position={s.position} icon={createShopIcon()}>
              <Popup>
                <strong>{s.name}</strong><br />
                {s.position}<br />
                <em>Shop Marker</em>
              </Popup>
            </Marker>
          ))}

          {/* 自訂標記 */}
          {customMarkers.map(m => (
            <Marker key={m.id} position={m.position} icon={createIcon('custom')}>
              <Popup>
                <strong>{m.name}</strong><br />
                {m.position}
              </Popup>
            </Marker>
          ))}

          <ChangeView center={center} />
          <ClickHandler onAdd={addCustomMarker} />
        </MapContainer>
      </div>
    </div>
  );
}