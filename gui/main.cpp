
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileInfo>
#include <QStringList>
#include <QAbstractItemView>
#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include <QSaveFile>
#include <QScrollArea>

#include <string>
#include <vector>

#include "FileManager.h"
#include "ComparisonManager.h"
#include "SimilarityPieChart.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;

    window.setStyleSheet(
    "QWidget {"
    "    background-color: #111827;"
    "    color: #F9FAFB;"
    "    font-family: Segoe UI;"
    "    font-size: 10pt;"
    "}"

    "QPushButton {"
    "    background-color: #2563EB;"
    "    color: white;"
    "    border: none;"
    "    border-radius: 6px;"
    "    padding: 8px 14px;"
    "    font-weight: bold;"
    "}"

    "QPushButton:hover {"
    "    background-color: #1D4ED8;"
    "}"

    "QPushButton:pressed {"
    "    background-color: #1E40AF;"
    "}"

    "QComboBox {"
    "    background-color: #1F2937;"
    "    color: white;"
    "    border: 1px solid #374151;"
    "    border-radius: 5px;"
    "    padding: 6px;"
    "}"

    "QLineEdit {"
    "    background-color: #1F2937;"
    "    color: white;"
    "    border: 1px solid #374151;"
    "    border-radius: 5px;"
    "    padding: 6px;"
    "}"

    "QLabel {"
    "    color: #E5E7EB;"
    "}"
);

    window.setWindowTitle("Automated Plagiarism Checker");
    window.resize(1100, 600);

    QLabel *title = new QLabel("Automated Plagiarism Checker");
    title->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #2563eb;"
    );

    QLabel *description = new QLabel(
        "Compare student files and identify textual similarity."
    );

    QPushButton *selectButton = new QPushButton("Select Folder");

    QLabel *folderLabel = new QLabel("No folder selected");
    folderLabel->setWordWrap(true);

    QLabel *ngramLabel = new QLabel("Select N-gram Size:");

    QComboBox *ngramSelector = new QComboBox();
    ngramSelector->addItem("2 words", 2);
    ngramSelector->addItem("3 words", 3);
    ngramSelector->addItem("5 words", 5);
    ngramSelector->setCurrentIndex(1);

    QLabel *filterLabel = new QLabel("Filter Results:");

    QComboBox *filterSelector = new QComboBox();
    filterSelector->addItem("All Categories");
    filterSelector->addItem("High Similarity");
    filterSelector->addItem("Medium Similarity");
    filterSelector->addItem("Low Similarity");

    QPushButton *checkButton = new QPushButton("Check Similarity");
    QPushButton *exportButton = new QPushButton("Export Results to CSV");

    QLabel *statusLabel = new QLabel(
        "Ready - select a folder to begin"
    );

    statusLabel->setStyleSheet(
        "color: #16a34a; font-size: 14px; font-weight: bold;"
    );

    // Similarity Summary Dashboard
    QLabel *totalLabel = new QLabel("Total Comparisons: 0");
    QLabel *highLabel = new QLabel("High Similarity: 0");
    QLabel *moderateLabel = new QLabel("Moderate Similarity: 0");
    QLabel *lowLabel = new QLabel("Low Similarity: 0");

    // Similarity Pie Chart
    SimilarityPieChart *pieChart = new SimilarityPieChart();    

    // Style the dashboard labels
    totalLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    highLabel->setStyleSheet("font-size: 14px; color: #dc2626;");
    moderateLabel->setStyleSheet("font-size: 14px; color: #d97706;");
    lowLabel->setStyleSheet("font-size: 14px; color: #16a34a;");

    // Create a table with five columns
    QTableWidget *table = new QTableWidget(0, 5);

    
table->setStyleSheet(
    "QTableWidget {"
    "    background-color: #1F2937;"
    "    alternate-background-color: #273449;"
    "    color: #F9FAFB;"
    "    gridline-color: #374151;"
    "    border: 1px solid #374151;"
    "    border-radius: 6px;"
    "    selection-background-color: #2563EB;"
    "    selection-color: white;"
    "}"

    "QHeaderView::section {"
    "    background-color: #1E3A5F;"
    "    color: white;"
    "    padding: 8px;"
    "    border: 1px solid #374151;"
    "    font-weight: bold;"
    "}"

    "QTableWidget::item {"
    "    padding: 6px;"
    "    border-bottom: 1px solid #374151;"
    "}"
);

table->setAlternatingRowColors(true);


    table->setHorizontalHeaderLabels(
        {"File 1", "File 2", "Similarity", "Category",
         "Matching Text Fragments"}
    );

    table->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setWordWrap(true);

    table->verticalHeader()->setSectionResizeMode(
    QHeaderView::Fixed
    );

    table->verticalHeader()->setDefaultSectionSize(100);

    QString selectedFolder;
    vector<Result> allResults;

    // Display results according to the selected category
    auto displayResults = [&]()
    {
        table->setRowCount(0);

        QString selectedFilter = filterSelector->currentText();
        int displayedCount = 0;

        for (const Result &result : allResults)
        {
            if (selectedFilter == "High Similarity" &&
                result.category != "High")
                continue;

            if (selectedFilter == "Medium Similarity" &&
                result.category != "Moderate" &&
                result.category != "Medium")
                continue;

            if (selectedFilter == "Low Similarity" &&
                result.category != "Low")
                continue;

            int row = table->rowCount();
            table->insertRow(row);
            displayedCount++;

            QString file1 = QFileInfo(
                QString::fromStdString(result.file1)
            ).fileName();

            QString file2 = QFileInfo(
                QString::fromStdString(result.file2)
            ).fileName();

            table->setItem(
                row, 0, new QTableWidgetItem(file1)
            );

            table->setItem(
                row, 1, new QTableWidgetItem(file2)
            );

            table->setItem(
                row, 2,
                new QTableWidgetItem(
                    QString::number(result.similarity, 'f', 2) + "%"
                )
            );

            table->setItem(
                row, 3,
                new QTableWidgetItem(
                    QString::fromStdString(result.category)
                )
            );

            QStringList fragments;

            for (const string &gram : result.matchingNGrams)
            {
                fragments.append(QString::fromStdString(gram));
            }

            QString matchingText = fragments.isEmpty()
                ? "No matching fragments"
                : fragments.join("\n");

            table->setItem(
                row, 4, new QTableWidgetItem(matchingText)
            );
        }

        statusLabel->setText(
            QString("%1 comparison results displayed")
                .arg(displayedCount)
        );

        statusLabel->setStyleSheet(
            "color: #16a34a; font-size: 14px; font-weight: bold;"
        );
    };

    // Update the table immediately when the filter changes
    QObject::connect(
        filterSelector,
        &QComboBox::currentTextChanged,
        [&displayResults](const QString &)
        {
            displayResults();
        }
    );

    // Create a scrollable page
    QScrollArea *scrollArea = new QScrollArea(&window);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAsNeeded
    );
    scrollArea->setVerticalScrollBarPolicy(
        Qt::ScrollBarAsNeeded
    );

    // Create the content inside the scroll area
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);

    // Place the content widget inside the scroll area
    scrollArea->setWidget(contentWidget);

    // Create the main window layout
    QVBoxLayout *windowLayout = new QVBoxLayout(&window);
    windowLayout->addWidget(scrollArea);

    layout->addWidget(title);
    layout->addWidget(description);
    layout->addSpacing(15);
    layout->addWidget(selectButton);
    layout->addWidget(folderLabel);
    layout->addWidget(ngramLabel);
    layout->addWidget(ngramSelector);
    layout->addWidget(filterLabel);
    layout->addWidget(filterSelector);

    // Similarity Summary Dashboard
    layout->addWidget(totalLabel);
    layout->addWidget(highLabel);
    layout->addWidget(moderateLabel);
    layout->addWidget(lowLabel);

    layout->addWidget(pieChart);

    layout->addWidget(statusLabel);
    layout->addWidget(checkButton);
    layout->addWidget(exportButton);
    table->setMinimumHeight(250);
    layout->addWidget(table, 1);

    // Select Folder button
    QObject::connect(
        selectButton, &QPushButton::clicked,
        [&window, &selectedFolder, &allResults,
         folderLabel, table, statusLabel]()
        {
            QString folder = QFileDialog::getExistingDirectory(
                &window, "Select Folder"
            );

            if (!folder.isEmpty())
            {
                selectedFolder = folder;
                allResults.clear();

                folderLabel->setText(folder);
                table->setRowCount(0);

                statusLabel->setText(
                    "Folder selected - ready to compare"
                );

                statusLabel->setStyleSheet(
                    "color: #2563eb; font-size: 14px; font-weight: bold;"
                );
            }
        }
    );

    // Check Similarity button
    QObject::connect(
        checkButton, &QPushButton::clicked,
        [&window, &selectedFolder, &allResults, &displayResults,
            table, statusLabel, ngramSelector,
            totalLabel, highLabel, moderateLabel, lowLabel,pieChart]()
        {
            if (selectedFolder.isEmpty())
            {
                statusLabel->setText(
                    "Please select a folder first"
                );

                statusLabel->setStyleSheet(
                    "color: #dc2626; font-size: 14px; font-weight: bold;"
                );

                QMessageBox::warning(
                    &window, "Select Folder",
                    "Please select a folder first."
                );

                return;
            }

            table->setRowCount(0);

            statusLabel->setText("Comparing files...");
            statusLabel->setStyleSheet(
                "color: #d97706; font-size: 14px; font-weight: bold;"
            );

            QApplication::processEvents();

            FileManager fileManager;

            vector<string> files =
                fileManager.findFiles(selectedFolder.toStdString());

            if (files.size() < 2)
            {
                allResults.clear();

                statusLabel->setText(
                    "Not enough files to compare"
                );

                statusLabel->setStyleSheet(
                    "color: #dc2626; font-size: 14px; font-weight: bold;"
                );

                QMessageBox::information(
                    &window, "Not Enough Files",
                    "Select a folder containing at least two readable files."
                );

                return;
            }

            ComparisonManager comparison;

            // Get the selected n-gram size
            int n = ngramSelector->currentData().toInt();

            allResults = comparison.compareFiles(files, n);

            // Update Similarity Summary Dashboard
    int highCount = 0;
    int moderateCount = 0;
    int lowCount = 0;

    for (const Result &result : allResults)
    {
        if (result.category == "High")
            highCount++;
        else if (result.category == "Moderate" ||
                result.category == "Medium")
            moderateCount++;
        else if (result.category == "Low")
            lowCount++;
    }

    totalLabel->setText(
       QString("Total Comparisons: %1").arg(allResults.size())
    );

    highLabel->setText(
        QString("High Similarity: %1").arg(highCount)
    );

    moderateLabel->setText(
        QString("Moderate Similarity: %1").arg(moderateCount)
    );

    lowLabel->setText(
        QString("Low Similarity: %1").arg(lowCount)
    );

        // Update the pie chart
        pieChart->setCounts(
            highCount,
            moderateCount,
            lowCount
        );

            if (allResults.empty())
            {
                statusLabel->setText(
                    "No comparison results available"
                );

                statusLabel->setStyleSheet(
                    "color: #dc2626; font-size: 14px; font-weight: bold;"
                );

                QMessageBox::warning(
                    &window, "No Results",
                    "No comparisons could be completed. "
                    "Check that the selected files contain readable text."
                );

                return;
            }

            // Display results using the currently selected filter
            displayResults();

            QMessageBox::information(
                &window, "Comparison Complete",
                QString(
                    "Compared %1 file pairs.\n"
                    "Text and JSON reports have been saved."
                ).arg(allResults.size())
            );
        }
    );

    // Export Results to CSV
QObject::connect(
    exportButton, &QPushButton::clicked,
    [&window, &allResults]()
    {
        if (allResults.empty())
        {
            QMessageBox::warning(
                &window,
                "No Results",
                "Please check similarity before exporting results."
            );
            return;
        }

            QString fileName = QFileDialog::getSaveFileName(
                &window,
                "Export Results to CSV",
                "plagiarism_results.csv",
                "CSV Files (*.csv)"
            );

            if (fileName.isEmpty())
                return;

            if (!fileName.endsWith(".csv", Qt::CaseInsensitive))
                fileName += ".csv";

            QSaveFile file(fileName);

            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::critical(
                    &window,
                    "Export Failed",
                    "Could not create the CSV file."
                );
                return;
            }

            QTextStream out(&file);

            // Write the CSV header
            out << "\"File 1\",\"File 2\",\"Similarity (%)\","
                "\"Category\",\"Matching Text Fragments\"\n";

            // Export every comparison result
            for (const Result &result : allResults)
            {
                QString file1 = QFileInfo(
                    QString::fromStdString(result.file1)
                ).fileName();

                QString file2 = QFileInfo(
                    QString::fromStdString(result.file2)
                ).fileName();

                QStringList fragments;

                for (const string &gram : result.matchingNGrams)
                {
                    fragments.append(QString::fromStdString(gram));
                }

                QString matchingText = fragments.isEmpty()
                    ? "No matching fragments"
                    : fragments.join("; ");

                // Escape double quotes for valid CSV
                file1.replace("\"", "\"\"");
                file2.replace("\"", "\"\"");
                matchingText.replace("\"", "\"\"");

                QString category = QString::fromStdString(result.category);
                category.replace("\"", "\"\"");

                out << "\"" << file1 << "\","
                    << "\"" << file2 << "\","
                    << QString::number(result.similarity, 'f', 2) << ","
                    << "\"" << category << "\","
                    << "\"" << matchingText << "\"\n";
            }

            if (!file.commit())
            {
                QMessageBox::critical(
                    &window,
                    "Export Failed",
                    "Could not save the CSV file."
                );
                return;
            }

                QMessageBox::information(
                &window,
                "Export Successful",
                "Comparison results exported successfully."
            );
        }
    );

    window.show();

    return app.exec();
}
