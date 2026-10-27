# Architecture — CSV export status

```mermaid
flowchart TD
  A[Comparison results] --> B[Current implemented exporters]
  B --> C[TXT: saveReport]
  B --> D[JSON: saveJsonReport]
  E[CSV exporter] -.-> F[Not implemented in current source]
  style E stroke-dasharray: 5 5
  style F stroke-dasharray: 5 5
```

No `saveCsvReport` method, CSV writer, or `.csv` output appears in the inspected source. This diagram records the implementation gap rather than inventing a CSV module.
