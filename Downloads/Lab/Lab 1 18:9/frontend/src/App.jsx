import React from 'react';
import './App.css';
import MassConvert
    from './massConvert';

function App() {
    return (
        <div className="App">
            <div className="converter-card">
                <h1 className="app-title">
                    Yeung Man
                </h1>
                <h3 className="converter-title">
                    Mass Converter
                </h3>
                <MassConvert />
            </div>
        </div>
    );
}

export default App;