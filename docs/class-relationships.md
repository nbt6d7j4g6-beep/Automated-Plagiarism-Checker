# Complete class relationship diagram

```mermaid
classDiagram
 direction LR
 class ComparisonManager {
   +compareFiles(files: vector~string~) void
 }
 class FileManager {
   +findFiles(folderPath: string) vector~string~
   +readFile(filePath: string) string
 }
 class TextProcessor {
   +processText(text: string) string
 }
 class NGramGenerator {
   +generateNGrams(text: string, n: int) vector~string~
 }
 class SimilarityCalculator {
   +calculateSimilarity(gramsA: vector~string~, gramsB: vector~string~) double
 }
 class PlagiarismReport {
   -results: vector~Result~
   +addResult(file1: string, file2: string, similarity: double) void
   +saveReport(fileName: string) void
   +saveJsonReport(fileName: string) void
 }
 class Result {
   +file1: string
   +file2: string
   +similarity: double
   +category: string
 }
 PlagiarismReport *-- "0..*" Result : stores
 ComparisonManager ..> FileManager : creates/uses
 ComparisonManager ..> TextProcessor : creates/uses
 ComparisonManager ..> NGramGenerator : creates/uses
 ComparisonManager ..> SimilarityCalculator : creates/uses
 ComparisonManager ..> PlagiarismReport : creates/uses
```

There is no inheritance between the project classes. Dashed arrows are dependencies because `ComparisonManager` constructs local helper objects inside `compareFiles`; these are not long-lived member fields. `PlagiarismReport` stores zero or more nested `Result` records.
