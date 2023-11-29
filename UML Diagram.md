#  UML Class Diagram

```mermaid
classDiagram

    class setup {
        connectToWifi()
}

    class loop {
        postWebhook()
}

    class GlobalVariables {
        char[] ssid
        char[] pass
        char[] server
        int status
        int keyIndex
        float zeroX
        float zeroY
        float zeroZ
        float zeroR
        float newX
        float newY
        float newZ
        float newR
        WiFiSSLClient client
        Timer time
}

    class postWebhook {
        string postBody
}

    class connectToWifi {
        string fv

}

    loop o-- postWebhook
    connectToWifi o-- setup
    loop o-- GlobalVariables
    setup o-- GlobalVariables
    connectToWifi o-- GlobalVariables
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
