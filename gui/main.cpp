#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
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
#include <QFrame>
#include <QProgressBar>
#include <QTimer>

#include <string>
#include <vector>
#include <algorithm>

#include "FileManager.h"
#include "ComparisonManager.h"
#include "SimilarityPieChart.h"

using namespace std;

// Creates a dashboard card with a title, large number and description.
QFrame *createDashboardCard(
    const QString &heading,
    const QString &description,
    const QString &numberColor,
    QLabel *&numberLabel,
    QWidget *parent = nullptr)
{
    QFrame *card = new QFrame(parent);

    card->setStyleSheet(
        "QFrame {"
        "    background-color: #1F2937;"
        "    border: 1px solid #374151;"
        "    border-radius: 10px;"
        "}"
    );

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(14, 12, 14, 12);
    cardLayout->setSpacing(6);

    QLabel *headingLabel = new QLabel(heading);
    headingLabel->setWordWrap(true);
    headingLabel->setStyleSheet(
        "QLabel {"
        "    background: transparent;"
        "    border: none;"
        "    color: #D1D5DB;"
        "    font-size: 10pt;"
        "    font-weight: bold;"
        "}"
    );

    numberLabel = new QLabel("0");
    numberLabel->setStyleSheet(
        QString(
            "QLabel {"
            "    background: transparent;"
            "    border: none;"
            "    color: %1;"
            "    font-size: 25pt;"
            "    font-weight: bold;"
            "}"
        ).arg(numberColor)
    );

    QLabel *descriptionLabel = new QLabel(description);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet(
        "QLabel {"
        "    background: transparent;"
        "    border: none;"
        "    color: #9CA3AF;"
        "    font-size: 9pt;"
        "}"
    );

    cardLayout->addWidget(headingLabel);
    cardLayout->addWidget(numberLabel);
    cardLayout->addWidget(descriptionLabel);
    cardLayout->addStretch();

    return card;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Automated Plagiarism Checker");
    window.resize(1100, 700);

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

        "QComboBox QAbstractItemView {"
        "    background-color: #1F2937;"
        "    color: white;"
        "    selection-background-color: #2563EB;"
        "}"

        "QLabel {"
        "    color: #E5E7EB;"
        "}"
    );

    // Application heading
    QLabel *title = new QLabel("Automated Plagiarism Checker");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #60A5FA;"
        "background: transparent;"
    );

    QLabel *description = new QLabel(
        "Compare student files and identify textual similarity."
    );

    description->setStyleSheet(
        "color: #9CA3AF; background: transparent;"
    );

    // Folder selection
    QPushButton *selectButton = new QPushButton("Select Folder");

    QLabel *folderLabel = new QLabel("No folder selected");
    folderLabel->setWordWrap(true);

    // N-gram selection
    QLabel *ngramLabel = new QLabel("Select N-gram Size:");

    QComboBox *ngramSelector = new QComboBox();
    ngramSelector->addItem("2 words", 2);
    ngramSelector->addItem("3 words", 3);
    ngramSelector->addItem("5 words", 5);
    ngramSelector->setCurrentIndex(1);

    // Result filtering
    QLabel *filterLabel = new QLabel("Filter Results:");

    QComboBox *filterSelector = new QComboBox();
    filterSelector->addItem("All Categories");
    filterSelector->addItem("High Similarity");
    filterSelector->addItem("Medium Similarity");
    filterSelector->addItem("Low Similarity");

    // Result sorting
QLabel *sortLabel = new QLabel("Sort Results:");

QComboBox *sortSelector = new QComboBox();
sortSelector->addItem("Highest Similarity First");
sortSelector->addItem("Lowest Similarity First");

    QPushButton *checkButton =
        new QPushButton("Check Similarity");

    QPushButton *exportButton =
        new QPushButton("Export Results to CSV");

    QLabel *statusLabel = new QLabel(
        "Ready - select a folder to begin"
    );

    // Progress bar for file comparison
QProgressBar *progressBar = new QProgressBar();

progressBar->setRange(0, 100);
progressBar->setValue(0);
progressBar->setTextVisible(true);
progressBar->setFormat("%p% completed");
progressBar->setMinimumHeight(22);

progressBar->setStyleSheet(
    "QProgressBar {"
    "    background-color: #1F2937;"
    "    color: white;"
    "    border: 1px solid #374151;"
    "    border-radius: 5px;"
    "    text-align: center;"
    "}"
    "QProgressBar::chunk {"
    "    background-color: #2563EB;"
    "    border-radius: 4px;"
    "}"
);

    statusLabel->setStyleSheet(
        "color: #4ADE80;"
        "font-size: 14px;"
        "font-weight: bold;"
        "background: transparent;"
    );

    // Dashboard number labels
    QLabel *totalNumber = nullptr;
    QLabel *highNumber = nullptr;
    QLabel *moderateNumber = nullptr;
    QLabel *lowNumber = nullptr;

    // Create the four dashboard cards
    QFrame *totalCard = createDashboardCard(
        "Total Comparisons",
        "All file pairs",
        "#60A5FA",
        totalNumber
    );

    QFrame *highCard = createDashboardCard(
        "High Similarity",
        "Needs attention",
        "#F87171",
        highNumber
    );

    QFrame *moderateCard = createDashboardCard(
        "Moderate Similarity",
        "Review recommended",
        "#FBBF24",
        moderateNumber
    );

    QFrame *lowCard = createDashboardCard(
        "Low Similarity",
        "Lower similarity",
        "#4ADE80",
        lowNumber
    );

    // Similarity pie chart
    SimilarityPieChart *pieChart =
        new SimilarityPieChart();

    // Results table
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

    table->setHorizontalHeaderLabels({
        "File 1",
        "File 2",
        "Similarity",
        "Category",
        "Matching Text Fragments"
    });

    table->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    table->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    table->setWordWrap(true);

    table->verticalHeader()->setSectionResizeMode(
        QHeaderView::Fixed
    );

    table->verticalHeader()->setDefaultSectionSize(100);
    table->setMinimumHeight(250);

    // Data used by the comparison features
    QString selectedFolder;
    vector<Result> allResults;

    // Display results based on the selected category
    auto displayResults = [&]()
    {
        table->setRowCount(0);

        QString selectedFilter =
            filterSelector->currentText();

        vector<Result> sortedResults = allResults;

bool highestFirst =
    sortSelector->currentText() == "Highest Similarity First";

sort(sortedResults.begin(), sortedResults.end(),
    [highestFirst](const Result &a, const Result &b)
    {
        if (highestFirst)
            return a.similarity > b.similarity;

        return a.similarity < b.similarity;
    });

        int displayedCount = 0;

        for (const Result &result : sortedResults)
        {
            if (selectedFilter == "High Similarity" &&
                result.category != "High")
            {
                continue;
            }

            if (selectedFilter == "Medium Similarity" &&
                result.category != "Moderate" &&
                result.category != "Medium")
            {
                continue;
            }

            if (selectedFilter == "Low Similarity" &&
                result.category != "Low")
            {
                continue;
            }

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
                row, 0,
                new QTableWidgetItem(file1)
            );

            table->setItem(
                row, 1,
                new QTableWidgetItem(file2)
            );

            table->setItem(
                row, 2,
                new QTableWidgetItem(
                    QString::number(
                        result.similarity, 'f', 2
                    ) + "%"
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
                fragments.append(
                    QString::fromStdString(gram)
                );
            }

            QString matchingText = fragments.isEmpty()
                ? "No matching fragments"
                : fragments.join("\n");

            table->setItem(
                row, 4,
                new QTableWidgetItem(matchingText)
            );
        }

        statusLabel->setText(
            QString("%1 comparison results displayed")
                .arg(displayedCount)
        );

        statusLabel->setStyleSheet(
            "color: #4ADE80;"
            "font-size: 14px;"
            "font-weight: bold;"
            "background: transparent;"
        );
    };

    // Refresh the table when the filter changes
    QObject::connect(
        filterSelector,
        &QComboBox::currentTextChanged,
        [&displayResults](const QString &)
        {
            displayResults();
        }
    );

    QObject::connect(sortSelector, &QComboBox::currentTextChanged,
                 [&](const QString &)
                 {
                     displayResults();
                 });

    // Scrollable application page
    QScrollArea *scrollArea = new QScrollArea(&window);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAsNeeded
    );
    scrollArea->setVerticalScrollBarPolicy(
        Qt::ScrollBarAsNeeded
    );
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *contentWidget = new QWidget();
    contentWidget->setStyleSheet(
        "background-color: #111827;"
    );

    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(12);

    scrollArea->setWidget(contentWidget);

    QVBoxLayout *windowLayout = new QVBoxLayout(&window);
    windowLayout->addWidget(scrollArea);

    // Main page layout
    layout->addWidget(title);
    layout->addWidget(description);
    layout->addSpacing(8);
    layout->addWidget(selectButton);
    layout->addWidget(folderLabel);
    layout->addWidget(ngramLabel);
    layout->addWidget(ngramSelector);
    layout->addWidget(filterLabel);
    layout->addWidget(filterSelector);
    layout->addWidget(sortLabel);
    layout->addWidget(sortSelector);

    // Four summary cards in one row
    QHBoxLayout *dashboardLayout = new QHBoxLayout();
    dashboardLayout->setSpacing(12);

    dashboardLayout->addWidget(totalCard);
    dashboardLayout->addWidget(highCard);
    dashboardLayout->addWidget(moderateCard);
    dashboardLayout->addWidget(lowCard);

    layout->addLayout(dashboardLayout);

    layout->addWidget(pieChart);
    layout->addWidget(statusLabel);
    layout->addWidget(progressBar);
    layout->addWidget(checkButton);
    layout->addWidget(exportButton);
    layout->addWidget(table, 1);

    // Select Folder button
    QObject::connect(
        selectButton,
        &QPushButton::clicked,
        [&window, &selectedFolder, &allResults,
         folderLabel, table, statusLabel,
         totalNumber, highNumber, moderateNumber,
         lowNumber, pieChart]()
        {
            QString folder =
                QFileDialog::getExistingDirectory(
                    &window, "Select Folder"
                );

            if (!folder.isEmpty())
            {
                selectedFolder = folder;
                allResults.clear();

                folderLabel->setText(folder);
                table->setRowCount(0);

                // Reset the dashboard
                totalNumber->setText("0");
                highNumber->setText("0");
                moderateNumber->setText("0");
                lowNumber->setText("0");

                pieChart->setCounts(0, 0, 0);

                statusLabel->setText(
                    "Folder selected - ready to compare"
                );

                statusLabel->setStyleSheet(
                    "color: #60A5FA;"
                    "font-size: 14px;"
                    "font-weight: bold;"
                    "background: transparent;"
                );
            }
        }
    );

    // Check Similarity button
    QObject::connect(
        checkButton,
        &QPushButton::clicked,
        [&window, &selectedFolder, &allResults,
         &displayResults, table, statusLabel,
         ngramSelector, totalNumber, highNumber,
         moderateNumber, lowNumber, pieChart,progressBar]()
        {
            if (selectedFolder.isEmpty())
            {
                statusLabel->setText(
                    "Please select a folder first"
                );

                QMessageBox::warning(
                    &window,
                    "Select Folder",
                    "Please select a folder first."
                );

                return;
            }

            table->setRowCount(0);

            statusLabel->setText("Comparing files...");
            statusLabel->setStyleSheet(
                "color: #FBBF24;"
                "font-size: 14px;"
                "font-weight: bold;"
                "background: transparent;"
            );

            QApplication::processEvents();

            FileManager fileManager;

            vector<string> files =
                fileManager.findFiles(
                    selectedFolder.toStdString()
                );

            if (files.size() < 2)
            {
                allResults.clear();

                totalNumber->setText("0");
                highNumber->setText("0");
                moderateNumber->setText("0");
                lowNumber->setText("0");

                pieChart->setCounts(0, 0, 0);

                statusLabel->setText(
                    "Not enough files to compare"
                );

                QMessageBox::information(
                    &window,
                    "Not Enough Files",
                    "Select a folder containing at least "
                    "two readable files."
                );

                return;
            }

            ComparisonManager comparison;

            int n = ngramSelector->currentData().toInt();

            
progressBar->setValue(0);

allResults = comparison.compareFiles(
    files,
    n,
    [progressBar, statusLabel](int completed, int total)
    {
        if (total > 0)
        {
            int percentage = completed * 100 / total;
            progressBar->setValue(percentage);

            statusLabel->setText(
                QString("Comparing file pair %1 of %2...")
                    .arg(completed)
                    .arg(total)
            );

            QApplication::processEvents();
        }
    }
);


            // Count results by category
            int highCount = 0;
            int moderateCount = 0;
            int lowCount = 0;

            for (const Result &result : allResults)
            {
                if (result.category == "High")
                {
                    highCount++;
                }
                else if (result.category == "Moderate" ||
                         result.category == "Medium")
                {
                    moderateCount++;
                }
                else if (result.category == "Low")
                {
                    lowCount++;
                }
            }

            // Update the large numbers inside the cards
            totalNumber->setText(
                QString::number(allResults.size())
            );

            highNumber->setText(
                QString::number(highCount)
            );

            moderateNumber->setText(
                QString::number(moderateCount)
            );

            lowNumber->setText(
                QString::number(lowCount)
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

                QMessageBox::warning(
                    &window,
                    "No Results",
                    "No comparisons could be completed. "
                    "Check that the selected files contain "
                    "readable text."
                );

                return;
            }

            displayResults();

            QMessageBox::information(
                &window,
                "Comparison Complete",
                QString(
                    "Compared %1 file pairs.\n"
                    "Text and JSON reports have been saved."
                ).arg(allResults.size())
            );
        }
    );

    // Export Results to CSV
    QObject::connect(
        exportButton,
        &QPushButton::clicked,
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
            {
                return;
            }

            if (!fileName.endsWith(".csv", Qt::CaseInsensitive))
            {
                fileName += ".csv";
            }

            QSaveFile file(fileName);

            if (!file.open(
                    QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::critical(
                    &window,
                    "Export Failed",
                    "Could not create the CSV file."
                );
                return;
            }

            QTextStream out(&file);

            // CSV header
            out << "\"File 1\",\"File 2\",\"Similarity (%)\","
                << "\"Category\",\"Matching Text Fragments\"\n";

            // Export all comparison results
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
                    fragments.append(
                        QString::fromStdString(gram)
                    );
                }

                QString matchingText = fragments.isEmpty()
                    ? "No matching fragments"
                    : fragments.join("; ");

                // Escape double quotes for valid CSV
                file1.replace("\"", "\"\"");
                file2.replace("\"", "\"\"");
                matchingText.replace("\"", "\"\"");

                QString category =
                    QString::fromStdString(result.category);
                category.replace("\"", "\"\"");

                out << "\"" << file1 << "\","
                    << "\"" << file2 << "\","
                    << QString::number(
                        result.similarity, 'f', 2
                    ) << ","
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
