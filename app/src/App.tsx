import React from "react";
import { ReactComponent as Logo } from "./logo.svg";
import "./App.css";
import { PirSensor } from "./sensor";

const App: React.FC = () => {
  return (
    <div className="App">
      <header className="App-header">
        <Logo className="App-logo" />
        <p>An ESP32 and pir sensor app</p>
      </header>
      <PirSensor />
    </div>
  );
};

export default App;
