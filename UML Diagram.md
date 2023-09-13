#  UML Class Diagram

```mermaid
classDiagram

    class Setup {
        Serial.begin()
}

    class Loop {
        int dispX
        int dispY
        int dispZ
        int currentWait
        delay()

}

    class GlobalVariables {
        const int xPin
        const int yPin
        const int zPin
        const int waitForCycle
        bool motionDetected
        bool alarmSetOff
        int currX
        int currY
        int currZ
}

    class AlertSystem {
        
}

    Loop o-- AlertSystem
    Loop o-- GlobalVariables
    Setup o-- GlobalVariables
```


# Flow Chart

```mermaid
---
title: Flow Chart
---
stateDiagram-v2
    ProgramStart --> Initialize
    Initialize --> startBufferTimer
    Initialize --> Loop
    Loop --> checkStatus
    checkStatus --> Loop : Movement Detected
    checkStatus --> checkTimer : Movement Not Detected
    
    checkTimer --> Loop : Buffer Timer Not Elapsed
    checkTimer --> notifyUser : Buffer Timer Elapsed
    
```
