import React, { useEffect, useState } from 'react';

interface TelemetryPoint {
  timestamp: number;
  jitterNs: number;
  noiseDbm: number;
}

interface PacketLossChartProps {
  latestTelemetry: TelemetryPoint | null;
}

const PacketLossChart: React.FC<PacketLossChartProps> = ({ latestTelemetry }) => {
  const [dataPoints, setDataPoints] = useState<TelemetryPoint[]>([]);
  const maxDataPoints = 50;

  // Append new telemetry to our rolling window
  useEffect(() => {
    if (latestTelemetry) {
      setDataPoints((prev) => {
        const newData = [...prev, latestTelemetry];
        if (newData.length > maxDataPoints) {
          return newData.slice(newData.length - maxDataPoints);
        }
        return newData;
      });
    }
  }, [latestTelemetry]);

  // Calculate SVG Polyline points for the Jitter Graph
  const generateGraphPath = () => {
    if (dataPoints.length === 0) return '';
    
    const width = 600;
    const height = 150;
    const maxJitter = 150; // 150ns upper limit for visualization
    
    return dataPoints.map((point, index) => {
      const x = (index / (maxDataPoints - 1)) * width;
      // Invert Y axis for standard chart visual (0 at bottom)
      const y = height - (Math.min(point.jitterNs, maxJitter) / maxJitter) * height;
      return `${x},${y}`;
    }).join(' ');
  };

  return (
    <div className="bg-slate-900 rounded-xl p-6 border border-slate-700 shadow-xl">
      <div className="flex justify-between items-center mb-4">
        <h3 className="text-fuchsia-400 font-mono text-sm uppercase tracking-widest">
          ML Predictive Jitter Analysis
        </h3>
        <span className="text-slate-400 text-xs font-mono">
          Threshold: 50ns
        </span>
      </div>

      <div className="relative w-full h-[150px] bg-slate-800 rounded border border-slate-600">
        <svg width="100%" height="100%" viewBox="0 0 600 150" preserveAspectRatio="none">
          {/* Danger Zone background */}
          <rect x="0" y="0" width="100%" height="50" fill="rgba(239, 68, 68, 0.1)" />
          
          {/* Dynamic Data Line */}
          <polyline
            fill="none"
            stroke="#e879f9"
            strokeWidth="3"
            strokeLinejoin="round"
            points={generateGraphPath()}
          />
        </svg>

        {/* Real-time Overlay Indicator */}
        {dataPoints.length > 0 && (
          <div className="absolute top-2 right-4 text-right">
            <p className="text-2xl font-bold text-fuchsia-400 font-mono">
              {dataPoints[dataPoints.length - 1].jitterNs.toFixed(1)} ns
            </p>
            <p className="text-xs text-slate-400 uppercase">Current Delay</p>
          </div>
        )}
      </div>
    </div>
  );
};

export default PacketLossChart;
