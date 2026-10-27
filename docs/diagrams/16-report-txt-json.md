# Architecture — report generation and TXT/JSON export

```mermaid
flowchart TD
  A[Pair paths and similarity] --> B[PlagiarismReport.addResult]
  B --> C[Assign category thresholds]
  C --> D[Store Result in results vector]
  D --> E[saveReport: .txt]
  D --> F[saveJsonReport: .json]
  E --> G[Write title, file names, percentage, category]
  F --> H[Write project, n_gram_size=3, results array]
  H --> I[Escape quotes, backslashes, newline, carriage return, tab]
  G --> J[Close output file]
  I --> K[Close output file]
```

Categories in the code: below 30 = Low; 30 to below 60 = Moderate; 60 to below 80 = High; 80 or above = Very High. File-open failures while saving cause the save method to return without a message.
