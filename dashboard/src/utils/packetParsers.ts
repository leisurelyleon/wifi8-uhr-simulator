// Define the exact shape we expect from main.cpp's JSON payload
export interface EngineTelemetryPayload {
  tick: number;
  noise_dbm: number;
  predicted_jitter_ns: number;
}

// Define the shape our React components will actually use
export interface ChartDataPoint {
  timestamp: number;
  jitterNs: number;
  noiseDbm: number;
}

/**
 * Safely parses the raw WebSocket string from the C++ engine.
 * Protects the frontend from crashing if the backend sends a malformed packet.
 */
export const parseEngineTelemetry = (rawPayload: string): ChartDataPoint | null => {
  try {
    const parsed: Partial<EngineTelemetryPayload> = JSON.parse(rawPayload);

    // Strict validation: Ensure all required fields exist and are numbers
    if (
      typeof parsed.tick !== 'number' ||
      typeof parsed.noise_dbm !== 'number' ||
      typeof parsed.predicted_jitter_ns !== 'number'
    ) {
      console.warn('[PARSER] Malformed telemetry packet dropped:', rawPayload);
      return null;
    }

    // Map the C++ snake_case to TypeScript camelCase and add a JS timestamp
    return {
      timestamp: Date.now(),
      jitterNs: parsed.predicted_jitter_ns,
      noiseDbm: parsed.noise_dbm,
    };
  } catch (error) {
    console.error('[PARSER] JSON Parsing error. Raw string:', rawPayload);
    return null;
  }
};

/**
 * Utility to calculate standard deviations for jitter analysis.
 * Useful if you want to expand the ControlPanel to show long-term stability.
 */
export const calculateJitterVariance = (dataPoints: ChartDataPoint[]): number => {
  if (dataPoints.length === 0) return 0;

  const mean = dataPoints.reduce((acc, curr) => acc + curr.jitterNs, 0) / dataPoints.length;
  
  const squareDiffs = dataPoints.map((point) => {
    const diff = point.jitterNs - mean;
    return diff * diff;
  });

  const avgSquareDiff = squareDiffs.reduce((acc, curr) => acc + curr, 0) / squareDiffs.length;
  
  return Math.sqrt(avgSquareDiff);
};
