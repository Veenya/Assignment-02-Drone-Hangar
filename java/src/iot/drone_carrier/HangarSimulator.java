package iot.drone_carrier;

import java.util.concurrent.atomic.AtomicBoolean;

public class HangarSimulator {

    // Same codes you use in MonitoringAgent
    private volatile int droneCode = MonitoringAgent.INSIDE;
    private volatile int hangarCode = MonitoringAgent.NORMAL;
    private volatile float groundDistance = 0f;

    // Behavior tuning
    private final float climbRate = 10f;   // units per tick
    private final float descendRate = 12f; // units per tick
    private final float maxDist = 120f;

    private final AtomicBoolean alarmQueued = new AtomicBoolean(false);

    /** Called periodically by MonitoringAgent. Returns one message each time. */
    public String nextMsgTick() {
        // If alarm was requested, emit it once
        if (alarmQueued.compareAndSet(true, false)) {
            hangarCode = MonitoringAgent.ALLARM;
            return "ALARM,1"; // payload can be whatever you want
        }

        // Simple “hangar behavior” state machine
        switch (droneCode) {
            case MonitoringAgent.INSIDE:
                groundDistance = 0f;
                break;

            case MonitoringAgent.OPERATING:
                // Drone is out: keep a plausible distance (or oscillate)
                groundDistance = clamp(groundDistance + climbRate, 0f, maxDist);
                break;

            case MonitoringAgent.REQUEST_LANDING:
                // Simulate landing: go down to 0 then become INSIDE
                groundDistance = clamp(groundDistance - descendRate, 0f, maxDist);
                if (groundDistance <= 0.01f) {
                    groundDistance = 0f;
                    droneCode = MonitoringAgent.INSIDE;
                }
                break;

            case MonitoringAgent.REQUEST_TAKEOFF:
                // Simulate takeoff: go up a bit then become OPERATING
                groundDistance = clamp(groundDistance + climbRate, 0f, maxDist);
                if (groundDistance >= 30f) {
                    droneCode = MonitoringAgent.OPERATING;
                }
                break;

            default:
                break;
        }

        // Emit a standard STATE message matching your parser:
        // STATE,<drone>,<hangar>,<distance>
        return "STATE," + droneCode + "," + hangarCode + "," + groundDistance;
    }

    public void setDroneCode(int code) {
        this.droneCode = code;
        // Keep distance consistent when forcing state
        if (code == MonitoringAgent.INSIDE) groundDistance = 0f;
    }

    public void setHangarCode(int code) {
        this.hangarCode = code;
    }

    public void setGroundDistance(float d) {
        this.groundDistance = clamp(d, 0f, maxDist);
    }

    public void triggerAlarm() {
        alarmQueued.set(true);
    }

    private static float clamp(float v, float a, float b) {
        return Math.max(a, Math.min(b, v));
    }
}
