# Architecture — n-gram generation

```mermaid
flowchart TD
  A[Normalized text and n=3] --> B[Split by whitespace into words]
  B --> C{n <= 0 or word count < n?}
  C -- Yes --> D[Return empty n-gram vector]
  C -- No --> E[Start at word index 0]
  E --> F[Join n consecutive words with spaces]
  F --> G[Append n-gram]
  G --> H{Another valid start index?}
  H -- Yes --> E
  H -- No --> I[Return ordered n-gram vector]
```

For words `a b c d` and `n=3`, output is `a b c` and `b c d`. Repeated n-grams may occur in this vector, but the similarity calculator later converts them to sets.
