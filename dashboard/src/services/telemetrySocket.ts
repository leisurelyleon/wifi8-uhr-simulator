type TelemetrySubscriber = (data: any) => void;

class TelemetrySocketService {
  private socket: WebSocket | null = null;
  private url: string;
  private subscribers: Set<TelemetrySubscriber> = new Set();
  private reconnectAttempts = 0;
  private maxReconnectAttempts = 5;
  private isConnecting = false;

  constructor(url: string) {
    this.url = url;
  }

  // Establishes the connection and sets up self-healing logic
  public connect(): void {
    if (this.socket?.readyState === WebSocket.OPEN || this.isConnecting) {
      return;
    }

    this.isConnecting = true;
    console.log(`[SOCKET] Attempting connection to UHR Engine at ${this.url}...`);
    this.socket = new WebSocket(this.url);

    this.socket.onopen = () => {
      console.log('[SOCKET] Connected to 802.11bn Simulation Engine.');
      this.reconnectAttempts = 0; // Reset attempts on success
      this.isConnecting = false;
    };

    this.socket.onmessage = (event) => {
      this.broadcast(event.data);
    };

    this.socket.onclose = () => {
      console.warn('[SOCKET] Connection dropped. UHR Engine offline.');
      this.socket = null;
      this.isConnecting = false;
      this.attemptReconnect();
    };

    this.socket.onerror = (error) => {
      console.error('[SOCKET] Connection error encountered:', error);
      this.socket?.close();
    };
  }

  // Allows React components to listen for incoming data
  public subscribe(callback: TelemetrySubscriber): () => void {
    this.subscribers.add(callback);
    // Return an unsubscribe function for cleanup in React's useEffect
    return () => this.subscribers.delete(callback);
  }

  // Sends the raw string data to all listeners
  private broadcast(rawData: string): void {
    this.subscribers.forEach((callback) => callback(rawData));
  }

  // Self-healing backoff mechanism
  private attemptReconnect(): void {
    if (this.reconnectAttempts >= this.maxReconnectAttempts) {
      console.error('[SOCKET] Maximum reconnection attempts reached. Halting retries.');
      return;
    }

    this.reconnectAttempts++;
    const timeout = Math.min(1000 * Math.pow(2, this.reconnectAttempts), 10000); // Exponential backoff
    
    console.log(`[SOCKET] Attempting reconnect ${this.reconnectAttempts}/${this.maxReconnectAttempts} in ${timeout}ms...`);
    
    setTimeout(() => {
      this.connect();
    }, timeout);
  }

  // Send control commands back to the C++ engine (e.g., injecting interference)
  public sendCommand(command: object): void {
    if (this.socket && this.socket.readyState === WebSocket.OPEN) {
      this.socket.send(JSON.stringify(command));
    } else {
      console.warn('[SOCKET] Cannot send command: Engine not connected.');
    }
  }
}

// Export a single, shared instance for the whole application
export const engineSocket = new TelemetrySocketService('ws://localhost:8080');
