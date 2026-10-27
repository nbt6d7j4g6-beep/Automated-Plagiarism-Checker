# Class diagram — `TextProcessor`

```mermaid
classDiagram
class TextProcessor {
  +processText(text: string) string
}
```

**Access:** public method. **Attributes:** none declared. **Constructors/destructors:** none explicitly declared. Processing lowercases characters and collapses consecutive whitespace to a single space.
