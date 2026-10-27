
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

#include <string>
#include <vector>

#include "FileManager.h"
#include "ComparisonManager.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
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

    // Style the dashboard labels
    totalLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    highLabel->setStyleSheet("font-size: 14px; color: #dc2626;");
    moderateLabel->setStyleSheet("font-size: 14px; color: #d97706;");
    lowLabel->setStyleSheet("font-size: 14px; color: #16a34a;");

    // Create a table with five columns
    QTableWidget *table = new QTableWidget(0, 5);

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
        QHeaderView::ResizeToContents
    );

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

    QVBoxLayout *layout = new QVBoxLayout(&window);

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

    layout->addWidget(statusLabel);
    layout->addWidget(checkButton);
    layout->addWidget(table);

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
        totalLabel, highLabel, moderateLabel, lowLabel]()
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

    window.show();

    return app.exec();
}
