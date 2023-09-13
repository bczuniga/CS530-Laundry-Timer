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
flowchart
    ProgramStart --> Initialize
    Initialize --> Loop
    Loop --> checkStatus
    checkStatus --Movement Detected--> Loop
    checkStatus --Movement Not Detected--> checkTimer
    
    checkTimer --Buffer Timer Not Elapsed--> Loop
    checkTimer --Buffer Timer Elapsed--> notifyUser
    
```
