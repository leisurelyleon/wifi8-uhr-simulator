import React, { useEffect, useState } from 'react';

// Defines the spatial coordinates for our simulation nodes
interface Coordinate {
  x: number;
  y: number;
}

interface ApNode {
  id: string;
  pos: Coordinate;
  status: 'active' | 'idle' | 'interference';
}

interface NetworkGraphProps {
  activeJtLinks: string[]; // Array of AP IDs currently transmitting
  clientPos: Coordinate;
}

const NetworkGraph: React.FC<NetworkGraphProps> = ({ activeJtLinks, clientPos }) => {
  // Static layout of our 4 coordinated Access Points
  const [aps] = useState<ApNode[]>([
    { id: 'AP-1', pos: { x: 100, y: 100 }, status: 'active' },
    { id: 'AP-2', pos: { x: 500, y: 100 }, status: 'active' },
    { id: 'AP-3', pos: { x: 100, y: 300 }, status: 'active' },
    { id: 'AP-4', pos: { x: 500, y: 300 }, status: 'idle' },
  ]);

  return (
    <div className="bg-slate-900 rounded-xl p-6 border border-slate-700 shadow-2xl relative overflow-hidden">
      <h3 className="text-cyan-400 font-mono text-sm mb-4 uppercase tracking-widest">
        802.11bn Spatial Coordination Map
      </h3>
      
      {/* SVG Canvas for High-Performance Rendering */}
      <svg width="100%" height="400" className="bg-slate-800 rounded-lg">
        <defs>
          <radialGradient id="jt-beam" cx="50%" cy="50%" r="50%">
            <stop offset="0%" stopColor="rgba(34, 211, 238, 0.8)" />
            <stop offset="100%" stopColor="rgba(34, 211, 238, 0)" />
          </radialGradient>
        </defs>

        {/* Render Joint Transmission (JT) Beams */}
        {aps.map((ap) => (
          activeJtLinks.includes(ap.id) && (
            <line
              key={`link-${ap.id}`}
              x1={ap.pos.x}
              y1={ap.pos.y}
              x2={clientPos.x}
              y2={clientPos.y}
              stroke="url(#jt-beam)"
              strokeWidth="4"
              strokeDasharray="8, 4"
              className="animate-pulse"
            />
          )
        ))}

        {/* Render Access Points */}
        {aps.map((ap) => (
          <g key={ap.id} transform={`translate(${ap.pos.x}, ${ap.pos.y})`}>
            <circle 
              r="24" 
              fill={ap.status === 'interference' ? '#ef4444' : '#1e293b'} 
              stroke={ap.status === 'active' ? '#22d3ee' : '#64748b'} 
              strokeWidth="3" 
            />
            <text x="0" y="5" fill="#f8fafc" fontSize="12" textAnchor="middle" fontFamily="monospace">
              {ap.id}
            </text>
          </g>
        ))}

        {/* Render Target Client */}
        <g transform={`translate(${clientPos.x}, ${clientPos.y})`}>
          <circle r="12" fill="#f59e0b" className="animate-bounce" />
          <text x="0" y="25" fill="#f59e0b" fontSize="10" textAnchor="middle" fontFamily="monospace">
            UHR-CLIENT
          </text>
        </g>
      </svg>
    </div>
  );
};

export default NetworkGraph;
