import React from 'react';
import { MapContainer, TileLayer, Marker, Popup } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';
class App extends React.Component {
 constructor() {
 super();
 this.state = {
 lat: 51.505,
 lng: -0.09,
 zoom: 13,
 };
 }
 render() {
 const position = [this.state.lat, this.state.lng];
 return (
 <MapContainer 
 center={position} 
 zoom={this.state.zoom}
 scrollWheelZoom={false} 
 style={ {width:'350px', height:'350px'} }>
 <TileLayer
 attribution='&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> 
contributors'
 url='http://{s}.tile.osm.org/{z}/{x}/{y}.png'
 />
 <Marker position={position}>
 <Popup>
 <span>A pretty CSS3 popup. <br/> Easily customizable.</span>
 </Popup>
 </Marker>
 </MapContainer>
 );
 }
}
export default App;