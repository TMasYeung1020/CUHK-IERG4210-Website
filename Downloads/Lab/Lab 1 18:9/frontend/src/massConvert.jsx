import React, { useState } from "react";

function MassConvert() {
    const [mass, setMass] = useState("");
    const [scale, setScale] = useState("kilogram");
    const [toScale, setToScale] = useState("pound");
    const [error, setError] = useState("");

    const kgTolb = (kg) => kg / 0.4536;
    const lbTokg = (lb) => lb * 0.4536;
    const lbTog = (lb) => lb * 453.6;
    const gTolb = (g) => g / 453.6;

    const convertMass = (val, fromScale, toScale) => {
        if (fromScale === toScale) return val;

        if (fromScale === "kilogram" && toScale === "pound") return kgTolb(val);
        if (fromScale === "pound" && toScale === "kilogram") return lbTokg(val);
        if (fromScale === "pound" && toScale === "gram") return lbTog(val);
        if (fromScale === "gram" && toScale === "pound") return gTolb(val);
        if (fromScale === "gram" && toScale === "kilogram") return lbTokg(gTolb(val));
        if (fromScale === "kilogram" && toScale === "gram") return lbTog(kgTolb(val));

        return val;
    };

    const massChangeFunction = (event) => {
        const input = event.target.value;
        setMass(input);
        if (!/^-?\d*\.?\d*$/.test(input)) {
            setError("Enter a valid numeric value.");
        } else {
            setError("");
        }
    };

    const scaleChangeFunction = (event) => {
        const selectedScale = event.target.value;
        setScale(selectedScale);
        if (selectedScale === toScale) {
            setError("Conversion between the same scale is not allowed.");
        } else {
            setError("");
        }
    };

    const toScaleChangeFunction = (event) => {
        const selectedToScale = event.target.value;
        setToScale(selectedToScale);
        if (selectedToScale === scale) {
            setError("Conversion between the same scale is not allowed.");
        } else {
            setError("");
        }
    };

    const result = convertMass(Number(mass), scale, toScale);

    return (
        <div className="converter">
            <div className="input-group">
                <input
                    type="text"
                    className="form-control"
                    placeholder={`Enter ${scale}`}
                    value={mass}
                    onChange={massChangeFunction}
                />
                <select className="form-select" value={scale} onChange={scaleChangeFunction}>
                    <option value="kilogram">Kilogram (kg)</option>
                    <option value="pound">Pound (lb)</option>
                    <option value="gram">Gram (g)</option>
                </select>
                <span className="arrow">➡</span>
                <select className="form-select" value={toScale} onChange={toScaleChangeFunction}>
                    <option value="kilogram">Kilogram (kg)</option>
                    <option value="pound">Pound (lb)</option>
                    <option value="gram">Gram (g)</option>
                </select>
            </div>
            {error ? (
                <p className="error-text">
                    <i className="fa fa-exclamation-circle" />
                    {error}
                </p>
            ) : (
                <p className="result-text">
                    {result !== null ? (
                        <>
                            <i className="fa fa-balance-scale" />
                            {mass} {scale} = {result.toFixed(2)} {toScale}.
                        </>
                    ) : (
                        "Select different units to convert."
                    )}
                </p>
            )}
        </div>
    );
}

export default MassConvert;
