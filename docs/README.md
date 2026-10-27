# Automated Plagiarism Checker — Documentation Diagrams

These diagrams were prepared by inspecting the current `src/*.h` and `src/*.cpp` files. Mermaid diagrams render in GitHub Markdown.

## Individual class diagrams
- [ComparisonManager](diagrams/01-comparison-manager.md)
- [FileManager](diagrams/02-file-manager.md)
- [TextProcessor](diagrams/03-text-processor.md)
- [NGramGenerator](diagrams/04-ngram-generator.md)
- [SimilarityCalculator](diagrams/05-similarity-calculator.md)
- [PlagiarismReport and nested Result](diagrams/06-plagiarism-report.md)
- [`main()` entry point](diagrams/07-main-entry-point.md) — function, not a class

## Class relationships
- [Complete class relationship diagram](diagrams/08-class-relationships.md)

## Architecture diagrams
- [Overall application flow](diagrams/09-overall-architecture.md)
- [Qt GUI status](diagrams/10-qt-gui-status.md)
- [File discovery and reading](diagrams/11-file-discovery-reading.md)
- [Text processing](diagrams/12-text-processing.md)
- [N-gram generation](diagrams/13-ngram-generation.md)
- [Similarity calculation](diagrams/14-similarity-calculation.md)
- [Comparison management](diagrams/15-comparison-management.md)
- [Report generation and TXT/JSON export](diagrams/16-report-txt-json.md)
- [CSV export status](diagrams/17-csv-export-status.md)

## Scope and accuracy notes
- The current source contains six classes: `ComparisonManager`, `FileManager`, `TextProcessor`, `NGramGenerator`, `SimilarityCalculator`, and `PlagiarismReport`. `PlagiarismReport` contains a private nested `Result` struct.
- No constructors or destructors are explicitly declared; the diagrams list methods declared in the source.
- No inheritance exists between these project classes. `ComparisonManager` creates local helper objects inside `compareFiles`, so these are dependencies rather than persistent member composition.
- The current source has no Qt headers, widgets, signals/slots, or GUI implementation. The entry point is a console `main()` function.
- The current source exports TXT and JSON only; CSV export is not implemented.
- Similarity is calculated using sets of unique n-grams, with a fixed n-gram size of 3.
