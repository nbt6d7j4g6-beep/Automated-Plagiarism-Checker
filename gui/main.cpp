#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "FileManager.h"
#include "NGramGenerator.h"
#include "PlagiarismReport.h"
#include "SimilarityCalculator.h"
#include "TextProcessor.h"

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

namespace
{
struct ComparisonResult
{
    QString file1;
    QString file2;
    double similarity;
};

QString displayName(const std::string &path)
{
    return QString::fromStdString(std::filesystem::path(path).filename().string());
}
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Automated Plagiarism Checker");
    window.resize(900, 600);

    auto *title = new QLabel("Automated Plagiarism Checker");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #2563eb;");
    auto *description = new QLabel(
        "Compare source code and text files using the built-in 3-word n-gram similarity engine.");
    auto *selectButton = new QPushButton("Select Folder");
    auto *checkButton = new QPushButton("Check Similarity");
    checkButton->setEnabled(false);
    auto *folderLabel = new QLabel("No folder selected");
    folderLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    auto *statusLabel = new QLabel("Select a folder containing .txt, .cpp, .h, or .hpp files.");
    auto *table = new QTableWidget(0, 3);
    table->setHorizontalHeaderLabels({"File 1", "File 2", "Similarity"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto *buttonRow = new QHBoxLayout;
    buttonRow->addWidget(selectButton);
    buttonRow->addWidget(checkButton);
    buttonRow->addStretch();

    auto *layout = new QVBoxLayout(&window);
    layout->addWidget(title);
    layout->addWidget(description);
    layout->addSpacing(12);
    layout->addLayout(buttonRow);
    layout->addWidget(folderLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(table);

    auto selectedFolder = std::make_shared<QString>();

    QObject::connect(selectButton, &QPushButton::clicked, &window,
                     [&window, folderLabel, checkButton, statusLabel, table, selectedFolder]()
    {
        const QString folder = QFileDialog::getExistingDirectory(&window, "Select Folder");
        if (folder.isEmpty())
            return;

        *selectedFolder = folder;
        folderLabel->setText(folder);
        table->setRowCount(0);
        FileManager files;
        const auto paths = files.findFiles(QDir::toNativeSeparators(folder).toStdString());
        checkButton->setEnabled(paths.size() >= 2);
        statusLabel->setText(paths.size() < 2
            ? QString("Found %1 supported file(s); at least two are needed.").arg(paths.size())
            : QString("Found %1 supported file(s). Ready to compare.").arg(paths.size()));
    });

    QObject::connect(checkButton, &QPushButton::clicked, &window,
                     [&window, statusLabel, table, selectedFolder]()
    {
        try
        {
            FileManager fileManager;
            TextProcessor processor;
            NGramGenerator generator;
            SimilarityCalculator calculator;
            PlagiarismReport report;
            constexpr int nGramSize = 3;

            const auto paths = fileManager.findFiles(
                QDir::toNativeSeparators(*selectedFolder).toStdString());
            std::vector<ComparisonResult> results;

            for (std::size_t i = 0; i < paths.size(); ++i)
            {
                for (std::size_t j = i + 1; j < paths.size(); ++j)
                {
                    const std::string content1 = fileManager.readFile(paths[i]);
                    const std::string content2 = fileManager.readFile(paths[j]);
                    if (content1.empty() || content2.empty())
                        continue;

                    const auto grams1 = generator.generateNGrams(processor.processText(content1), nGramSize);
                    const auto grams2 = generator.generateNGrams(processor.processText(content2), nGramSize);
                    const double similarity = calculator.calculateSimilarity(grams1, grams2);

                    report.addResult(paths[i], paths[j], similarity);
                    results.push_back({displayName(paths[i]), displayName(paths[j]), similarity});
                }
            }

            std::sort(results.begin(), results.end(), [](const auto &a, const auto &b)
            {
                return a.similarity > b.similarity;
            });

            table->setRowCount(static_cast<int>(results.size()));
            for (int row = 0; row < static_cast<int>(results.size()); ++row)
            {
                table->setItem(row, 0, new QTableWidgetItem(results[row].file1));
                table->setItem(row, 1, new QTableWidgetItem(results[row].file2));
                table->setItem(row, 2, new QTableWidgetItem(
                    QString::number(results[row].similarity, 'f', 2) + "%"));
            }

            const QString reportFolder = QDir(*selectedFolder).filePath("plagiarism_report");
            report.saveReport(reportFolder.toStdString() + ".txt");
            report.saveJsonReport(reportFolder.toStdString() + ".json");
            statusLabel->setText(QString("Compared %1 file pair(s). Reports saved in the selected folder.")
                                     .arg(results.size()));
            if (results.empty())
                QMessageBox::information(&window, "No comparisons", "No readable file pairs were found.");
        }
        catch (const std::exception &error)
        {
            QMessageBox::critical(&window, "Comparison failed", QString::fromLocal8Bit(error.what()));
        }
    });

    window.show();
    return app.exec();
}
