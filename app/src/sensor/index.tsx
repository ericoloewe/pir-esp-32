import React from "react";
import "./index.scss";

interface Props {}
interface State {
  sensorStatus: string;
  wsStatus: string;
}

const url = `ws://localhost:5000/ws`;
let socket: WebSocket;

export class PirSensor extends React.Component<Props, State> {
  state = {
    sensorStatus: "No signal...",
    wsStatus: "No signal..."
  };

  componentDidMount() {
    socket = new WebSocket(url);

    socket.onopen = () => {
      this.setState(() => ({ wsStatus: "opened" }));
      socket.send("Receive sensor status");
    };

    socket.onclose = () => this.setState(() => ({ wsStatus: "closed" }));

    socket.onerror = e => {
      this.setState(() => ({ wsStatus: "error" }));
      console.error("Stack: ", e);
    };

    socket.onmessage = ({ data }) =>
      this.setState(() => ({ wsStatus: "message", sensorStatus: data }));
  }

  render() {
    const { sensorStatus, wsStatus } = this.state;

    return (
      <section className="pir-sensor">
        <p>Connecting to url: {url}</p>
        <p>The sensor status is: {sensorStatus}</p>
        <p>The ws status is: {wsStatus}</p>
      </section>
    );
  }
}
