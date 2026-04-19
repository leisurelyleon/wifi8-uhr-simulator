import React, { useState } from 'react';

interface ControlPanelProps {
  onInjectInterference: (intensity: number) => void;
  onToggleLosBlockage: (isBlocked: boolean) => void;
}

const ControlPanel: React.FC<ControlPanelProps> = ({ onInjectInterference, onToggleLosBlockage }) => {
  const [interferenceLevel, setInterferenceLevel] = useState<number>(0);
  const [losBlocked, setLosBlocked] = useState<boolean>(false);

  const handleSliderChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const val = parseInt(e.target.value, 10);
    setInterferenceLevel(val);
    onInjectInterference(val);
  };

  const handleLosToggle = () => {
    const newState = !losBlocked;
    setLosBlocked(newState);
    onToggleLosBlockage(newState);
  };

  return (
    <div className="bg-slate-900 rounded-xl p-6 border border-slate-700 shadow-xl flex flex-col gap-6">
      <h3 className="text-emerald-400 font-mono text-sm uppercase tracking-widest border-b border-slate-700 pb-2">
        Environment Stress Controls
      </h3>

      {/* Interference Slider */}
      <div className="flex flex-col gap-2">
        <div className="flex justify-between font-mono text-sm">
          <span className="text-slate-300">Ambient RF Noise</span>
          <span className={interferenceLevel > 70 ? 'text-red-400' : 'text-emerald-400'}>
            {interferenceLevel}%
          </span>
        </div>
        <input
          type="range"
          min="0"
          max="100"
          value={interferenceLevel}
          onChange={handleSliderChange}
          className="w-full h-2 bg-slate-700 rounded-lg appearance-none cursor-pointer accent-emerald-500"
        />
        <p className="text-xs text-slate-500 font-mono mt-1">
          Simulates overlapping networks (OBSS) causing BSS Color shifts.
        </p>
      </div>

      {/* Line of Sight Toggle */}
      <div className="flex flex-col gap-2 pt-4 border-t border-slate-800">
        <div className="flex justify-between items-center">
          <span className="text-slate-300 font-mono text-sm">Line of Sight (LOS) Status</span>
          <button
            onClick={handleLosToggle}
            className={`px-4 py-2 rounded font-mono text-xs font-bold transition-colors ${
              losBlocked 
                ? 'bg-red-500/20 text-red-400 border border-red-500' 
                : 'bg-emerald-500/20 text-emerald-400 border border-emerald-500'
            }`}
          >
            {losBlocked ? 'BLOCKED' : 'CLEAR'}
          </button>
        </div>
        <p className="text-xs text-slate-500 font-mono">
          Triggers Sub-THz fallback logic to the 6GHz baseline.
        </p>
      </div>

      {/* Emergency Stop */}
      <button className="mt-4 w-full bg-slate-800 hover:bg-red-600 border border-slate-600 hover:border-red-500 text-slate-300 hover:text-white font-mono text-sm py-3 rounded transition-all duration-200 uppercase tracking-widest">
        Halt Simulation Engine
      </button>
    </div>
  );
};

export default ControlPanel;
