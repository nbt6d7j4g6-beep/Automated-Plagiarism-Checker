# Architecture — Qt GUI status

```mermaid
flowchart TD
  A[Current source entry point: main.cpp] --> B[Console output via std::cout]
  B --> C[FileManager and ComparisonManager]
  C --> D[TXT and JSON report files]
  X[Qt GUI layer] -.->|not present in current source| A
  style X stroke-dasharray: 5 5
```

**Accuracy note:** No Qt GUI component exists in the supplied `.h`/`.cpp` source: there are no Qt includes, `QApplication`, widgets, signals/slots, or GUI classes. This is a status diagram, not a claim that a Qt interface is implemented. A future GUI could call the existing comparison layer, but that is outside the current code.
