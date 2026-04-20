import React, { useEffect, useState } from 'react';
import NetworkGraph from './components/NetworkGraph';
import PacketLossChart from './components/PacketLossChart';
import ControlPanel from './components/ControlPanel';
import { engineSocket } from './services/telemetrySocket';
import { parseEngineTelemetry, ChartDataPoint } from './utils/packetParsers';
import './index.css';

const App: React.FC = () => {
  const [telemetry, setTelemetry] = useState<ChartDataPoint | null>(null);
  
  // Default to two APs linked for Joint Transmission
  const [activeJtLinks, setActiveJtLinks] = useState<string[]>(['AP-1', 'AP-2']);
  
  // UHR Client coordinates (simulating a user moving through the spatial map)
  const [clientPos, setClientPos] = useState({ x: 300, y: 200 });

  useEffect(() => {
    // 1. Boot the WebSocket connection to the C++ Engine
    engineSocket.connect();

    // 2. Subscribe to the live telemetry stream
    const unsubscribe = engineSocket.subscribe((rawData: string) => {
      const parsedData = parseEngineTelemetry(rawData);
      
      if (parsedData) {
        setTelemetry(parsedData);
        
        // Simulate AP Joint Transmission shifts based on the engine's interference data
        if (parsedData.noiseDbm > -60) {
          // High noise: shift the active beamforming away from AP-2
          setActiveJtLinks(['AP-1', 'AP-3']); 
        } else {
          // Clear airspace: full UHR Multi-AP Coordination
          setActiveJtLinks(['AP-1', 'AP-2', 'AP-3']); 
        }
      }
    });

    // Cleanup the listener when the component unmounts
    return () => {
      unsubscribe();
    };
  }, []);

  // Handlers to pass user interaction back to the C++ Engine
  const handleInjectInterference = (intensity: number) => {
    engineSocket.sendCommand({ command: 'SET_INTERFERENCE', value: intensity });
  };

  const handleToggleLosBlockage = (isBlocked: boolean) => {
    engineSocket.sendCommand({ command: 'TOGGLE_LOS', value: isBlocked });
  };

  return (
    <div className="min-h-screen bg-slate-950 text-slate-200 font-sans p-8 selection:bg-cyan-500/30">
      <header className="mb-8 border-b border-slate-800 pb-4">
        <h1 className="text-3xl font-light tracking-tight text-white">
          <span className="font-bold text-cyan-400">IEEE 802.11bn</span> UHR Dashboard
        </h1>
        <p className="text-slate-400 font-mono text-sm mt-2">
          Multi-Access Point Coordination (MAPC) & Predictive Sub-THz Telemetry
        </p>
      </header>

      <main className="grid grid-cols-1 lg:grid-cols-3 gap-8">
        {/* Left Column: Visual Spatial Map */}
        <section className="lg:col-span-2 glass-panel rounded-2xl">
          <NetworkGraph activeJtLinks={activeJtLinks} clientPos={clientPos} />
        </section>

        {/* Right Column: Controls & Machine Learning Metrics */}
        <section className="flex flex-col gap-8">
          <div className="glass-panel rounded-2xl">
            <PacketLossChart latestTelemetry={telemetry} />
          </div>
          <div className="glass-panel rounded-2xl">
            <ControlPanel 
              onInjectInterference={handleInjectInterference} 
              onToggleLosBlockage={handleToggleLosBlockage} 
            />
          </div>
        </section>
      </main>
    </div>
  );
};

export default App;
