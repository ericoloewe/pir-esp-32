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
    this.loadSocket();
    this.requestPermitionIfNeed();
  }

  private loadSocket() {
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
    socket.onmessage = this.handleMessage;
  }

  private handleMessage = ({ data }: MessageEvent) => {
    this.setState(() => ({ wsStatus: "message", sensorStatus: data }));

    if (data === "MD") {
      if (Notification.permission === "granted") {
        // If it's okay let's create a notification
        new Notification("Movimento detectado!");
      } else {
        alert("Movimento detectado!");
      }
    }
  };

  private requestPermitionIfNeed() {
    if (Notification.permission !== "denied") {
      Notification.requestPermission(permission => {
        // If the user accepts, let's create a notification
        console.log("User permission: ", permission);
      });
    }
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
