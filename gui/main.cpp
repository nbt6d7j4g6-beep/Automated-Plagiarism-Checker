
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
#include <QLineEdit>
#include <QDialog>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QColor>
#include <QTextDocument>

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

// Opens a side-by-side comparison window for two files.
void showComparisonWindow(const Result &result)
{
    QFile file1(QString::fromStdString(result.file1));
    QFile file2(QString::fromStdString(result.file2));

    if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(
            nullptr, "File Error",
            "Could not open the first file:\n" + file1.fileName()
        );
        return;
    }

    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(
            nullptr, "File Error",
            "Could not open the second file:\n" + file2.fileName()
        );
        return;
    }

    QString text1 = QString::fromUtf8(file1.readAll());
    QString text2 = QString::fromUtf8(file2.readAll());

    file1.close();
    file2.close();

    QDialog dialog;
    dialog.setWindowTitle("Side-by-Side File Comparison");
    dialog.resize(1100, 650);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *similarityLabel = new QLabel(
        QString("Similarity: %1%    |    Category: %2")
            .arg(result.similarity, 0, 'f', 2)
            .arg(QString::fromStdString(result.category))
    );

    similarityLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #60A5FA;"
        "padding: 6px; background: transparent;"
    );

    layout->addWidget(similarityLabel);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    QPlainTextEdit *leftEditor = new QPlainTextEdit();
    QPlainTextEdit *rightEditor = new QPlainTextEdit();

    leftEditor->setReadOnly(true);
    rightEditor->setReadOnly(true);

    leftEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
    rightEditor->setLineWrapMode(QPlainTextEdit::NoWrap);

    leftEditor->setPlainText(text1);
    rightEditor->setPlainText(text2);

    const QString editorStyle =
        "QPlainTextEdit {"
        " background-color: #111827;"
        " color: #F9FAFB;"
        " font-family: Consolas;"
        " font-size: 10pt;"
        " selection-background-color: #2563EB;"
        "}";

    leftEditor->setStyleSheet(editorStyle);
    rightEditor->setStyleSheet(editorStyle);

    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    QLabel *leftFilename = new QLabel(
        QFileInfo(QString::fromStdString(result.file1)).fileName()
    );
    leftFilename->setStyleSheet(
        "font-weight: bold; color: #60A5FA; background: transparent;"
    );

    leftLayout->addWidget(leftFilename);
    leftLayout->addWidget(leftEditor);

    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    QLabel *rightFilename = new QLabel(
        QFileInfo(QString::fromStdString(result.file2)).fileName()
    );
    rightFilename->setStyleSheet(
        "font-weight: bold; color: #60A5FA; background: transparent;"
    );

    rightLayout->addWidget(rightFilename);
    rightLayout->addWidget(rightEditor);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    // Highlight matching fragments in both original files.
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QColor("#FDE047"));
    highlightFormat.setForeground(QColor("#111827"));

    auto highlightMatches = [&](QPlainTextEdit *editor)
    {
        QTextDocument *document = editor->document();
        QString fullText = document->toPlainText();

        // Avoid applying the same fragment repeatedly.
        vector<QString> processedFragments;

        for (const string &gram : result.matchingNGrams)
        {
            QString fragment = QString::fromStdString(gram).trimmed();

            if (fragment.isEmpty() ||
                find(processedFragments.begin(),
                     processedFragments.end(),
                     fragment) != processedFragments.end())
            {
                continue;
            }

            processedFragments.push_back(fragment);

            QStringList words = fragment.split(
                QRegularExpression("\\s+"),
                Qt::SkipEmptyParts
            );

            if (words.isEmpty())
                continue;

            QStringList escapedWords;

            for (const QString &word : words)
                escapedWords.append(QRegularExpression::escape(word));

            QString pattern = escapedWords.join("\\s+");

            QRegularExpression expression(
                pattern,
                QRegularExpression::CaseInsensitiveOption
            );

            if (!expression.isValid())
                continue;

            QRegularExpressionMatchIterator matches =
                expression.globalMatch(fullText);

            while (matches.hasNext())
            {
                QRegularExpressionMatch match = matches.next();

                QTextCursor cursor(document);
                cursor.setPosition(match.capturedStart());
                cursor.setPosition(
                    match.capturedEnd(),
                    QTextCursor::KeepAnchor
                );

                cursor.mergeCharFormat(highlightFormat);
            }
        }
    };

    highlightMatches(leftEditor);
    highlightMatches(rightEditor);

    dialog.exec();
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
        "    color: #F3F4F6;"
        "    font-family: 'Segoe UI';"
        "    font-size: 10pt;"
        "}"

        "QPushButton {"
        "    background-color: #2563EB;"
        "    color: #FFFFFF;"
        "    border: 1px solid #3B82F6;"
        "    border-radius: 7px;"
        "    padding: 9px 16px;"
        "    font-weight: 600;"
        "}"

        "QPushButton:hover {"
        "    background-color: #1D4ED8;"
        "}"

        "QPushButton:pressed {"
        "    background-color: #1E40AF;"
        "}"

        "QPushButton:disabled {"
        "    background-color: #374151;"
        "    color: #9CA3AF;"
        "    border-color: #374151;"
        "}"

        "QComboBox, QLineEdit {"
        "    background-color: #1F2937;"
        "    color: #F9FAFB;"
        "    border: 1px solid #4B5563;"
        "    border-radius: 7px;"
        "    padding: 8px 10px;"
        "    selection-background-color: #2563EB;"
        "}"

        "QComboBox:hover, QLineEdit:hover {"
        "    border-color: #60A5FA;"
        "}"

        "QComboBox:focus, QLineEdit:focus {"
        "    border: 1px solid #60A5FA;"
        "}"

        "QComboBox QAbstractItemView {"
        "    background-color: #1F2937;"
        "    color: #F9FAFB;"
        "    border: 1px solid #4B5563;"
        "    selection-background-color: #2563EB;"
        "    selection-color: #FFFFFF;"
        "}"

        "QLabel {"
        "    color: #E5E7EB;"
        "}"

        "QScrollBar:vertical {"
        "    background-color: #111827;"
        "    width: 10px;"
        "    margin: 0px;"
        "}"

        "QScrollBar::handle:vertical {"
        "    background-color: #4B5563;"
        "    min-height: 25px;"
        "    border-radius: 4px;"
        "}"

        "QScrollBar::handle:vertical:hover {"
        "    background-color: #6B7280;"
        "}"

        "QScrollBar::add-line:vertical,"
        "QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"

        "QToolTip {"
        "    background-color: #1F2937;"
        "    color: #F9FAFB;"
        "    border: 1px solid #4B5563;"
        "    padding: 5px;"
        "}"
    );

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

    QLabel *sortLabel = new QLabel("Sort Results:");

    QComboBox *sortSelector = new QComboBox();
    sortSelector->addItem("Highest Similarity First");
    sortSelector->addItem("Lowest Similarity First");

    QLabel *searchLabel = new QLabel("Search by Filename:");

    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Enter a filename...");

    QPushButton *checkButton = new QPushButton("Check Similarity");
    QPushButton *exportButton = new QPushButton("Export Results to CSV");

    QLabel *statusLabel = new QLabel("Ready - select a folder to begin");

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

    QLabel *totalNumber = nullptr;
    QLabel *highNumber = nullptr;
    QLabel *moderateNumber = nullptr;
    QLabel *lowNumber = nullptr;

    QFrame *totalCard = createDashboardCard(
        "Total Comparisons", "All file pairs", "#60A5FA", totalNumber
    );

    QFrame *highCard = createDashboardCard(
        "High Similarity", "Needs attention", "#F87171", highNumber
    );

    QFrame *moderateCard = createDashboardCard(
        "Moderate Similarity", "Review recommended",
        "#FBBF24", moderateNumber
    );

    QFrame *lowCard = createDashboardCard(
        "Low Similarity", "Lower similarity", "#4ADE80", lowNumber
    );

    SimilarityPieChart *pieChart = new SimilarityPieChart();

    // Instruction to help users discover the comparison feature
QLabel *comparisonTip = new QLabel(
    "Tip: Double-click any result row to open the side-by-side "
    "comparison window and view highlighted matching text."
);

comparisonTip->setWordWrap(true);

comparisonTip->setStyleSheet(
    "QLabel {"
    "    color: #60A5FA;"
    "    background-color: #1F2937;"
    "    border: 1px solid #374151;"
    "    border-radius: 6px;"
    "    padding: 10px;"
    "    font-size: 10pt;"
    "    font-weight: 600;"
    "}"
);

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

    table->setToolTip(
    "Double-click any result row to open the side-by-side "
    "comparison window and view highlighted matching text."
);

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

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setWordWrap(true);

    table->verticalHeader()->setSectionResizeMode(
        QHeaderView::Fixed
    );

    table->verticalHeader()->setDefaultSectionSize(100);
    table->setMinimumHeight(250);

    QString selectedFolder;
    vector<Result> allResults;

    // Display results according to filters, search and sorting.
    auto displayResults = [&]()
    {
        table->setRowCount(0);

        QString selectedFilter = filterSelector->currentText();
        QString searchText = searchInput->text().trimmed();

        vector<Result> sortedResults = allResults;

        bool highestFirst =
            sortSelector->currentText() == "Highest Similarity First";

        sort(
            sortedResults.begin(),
            sortedResults.end(),
            [highestFirst](const Result &a, const Result &b)
            {
                if (highestFirst)
                    return a.similarity > b.similarity;

                return a.similarity < b.similarity;
            }
        );

        int displayedCount = 0;

        for (const Result &result : sortedResults)
        {
            QString file1Name = QString::fromStdString(result.file1);
            QString file2Name = QString::fromStdString(result.file2);

            if (!searchText.isEmpty() &&
                !file1Name.contains(searchText, Qt::CaseInsensitive) &&
                !file2Name.contains(searchText, Qt::CaseInsensitive))
            {
                continue;
            }

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

            QString file1 = QFileInfo(file1Name).fileName();
            QString file2 = QFileInfo(file2Name).fileName();

            // Store full paths invisibly in the table items.
            // This allows the correct files to open after sorting/filtering.
            QTableWidgetItem *file1Item = new QTableWidgetItem(file1);
            file1Item->setData(Qt::UserRole, file1Name);
            table->setItem(row, 0, file1Item);

            QTableWidgetItem *file2Item = new QTableWidgetItem(file2);
            file2Item->setData(Qt::UserRole, file2Name);
            table->setItem(row, 1, file2Item);

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
                fragments.append(QString::fromStdString(gram));

            QString matchingText = fragments.isEmpty()
                ? "No matching fragments"
                : fragments.join("\n");

            table->setItem(
                row, 4,
                new QTableWidgetItem(matchingText)
            );
        }

        statusLabel->setText(
            QString("%1 comparison results displayed").arg(displayedCount)
        );

        statusLabel->setStyleSheet(
            "color: #4ADE80;"
            "font-size: 14px;"
            "font-weight: bold;"
            "background: transparent;"
        );
    };

    // NEW: Double-click a row to open the corresponding comparison.
    QObject::connect(
        table,
        &QTableWidget::cellDoubleClicked,
        [&allResults, table](int row, int)
        {
            QTableWidgetItem *file1Item = table->item(row, 0);
            QTableWidgetItem *file2Item = table->item(row, 1);

            if (file1Item == nullptr || file2Item == nullptr)
                return;

            QString file1Path =
                file1Item->data(Qt::UserRole).toString();

            QString file2Path =
                file2Item->data(Qt::UserRole).toString();

            for (const Result &result : allResults)
            {
                if (QString::fromStdString(result.file1) == file1Path &&
                    QString::fromStdString(result.file2) == file2Path)
                {
                    showComparisonWindow(result);
                    return;
                }
            }

            QMessageBox::warning(
                table,
                "Comparison Unavailable",
                "The comparison result could not be found."
            );
        }
    );

    QObject::connect(
        filterSelector,
        &QComboBox::currentTextChanged,
        [&displayResults](const QString &)
        {
            displayResults();
        }
    );

    QObject::connect(
        sortSelector,
        &QComboBox::currentTextChanged,
        [&displayResults](const QString &)
        {
            displayResults();
        }
    );

    QObject::connect(
        searchInput,
        &QLineEdit::textChanged,
        [&displayResults](const QString &)
        {
            displayResults();
        }
    );

    // Scrollable application page
    QScrollArea *scrollArea = new QScrollArea(&window);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: #111827;");

    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(12);

    scrollArea->setWidget(contentWidget);

    QVBoxLayout *windowLayout = new QVBoxLayout(&window);
    windowLayout->addWidget(scrollArea);

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
    layout->addWidget(searchLabel);
    layout->addWidget(searchInput);

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

    // Show the comparison instruction directly above the results
    layout->addWidget(comparisonTip);
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
            QString folder = QFileDialog::getExistingDirectory(
                &window, "Select Folder"
            );

            if (!folder.isEmpty())
            {
                selectedFolder = folder;
                allResults.clear();

                folderLabel->setText(folder);
                table->setRowCount(0);

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
         moderateNumber, lowNumber, pieChart, progressBar]()
        {
            if (selectedFolder.isEmpty())
            {
                statusLabel->setText("Please select a folder first");

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

            vector<string> files = fileManager.findFiles(
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

            totalNumber->setText(QString::number(allResults.size()));
            highNumber->setText(QString::number(highCount));
            moderateNumber->setText(QString::number(moderateCount));
            lowNumber->setText(QString::number(lowCount));

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

            out << "\"File 1\",\"File 2\",\"Similarity (%)\","
                << "\"Category\",\"Matching Text Fragments\"\n";

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
                    fragments.append(QString::fromStdString(gram));

                QString matchingText = fragments.isEmpty()
                    ? "No matching fragments"
                    : fragments.join("; ");

                file1.replace("\"", "\"\"");
                file2.replace("\"", "\"\"");
                matchingText.replace("\"", "\"\"");

                QString category =
                    QString::fromStdString(result.category);
                category.replace("\"", "\"\"");

                out << "\"" << file1 << "\","
                    << "\"" << file2 << "\","
                    << QString::number(result.similarity, 'f', 2) << ","
                    << "\"" << category << "\","
                    << "\"" << matchingText << "\"\n";
            }

            if (out.status() != QTextStream::Ok)
            {
                QMessageBox::critical(
                    &window,
                    "Export Failed",
                    "An error occurred while writing "
                    "the CSV file. The file was not saved."
                );

                file.cancelWriting();
                return;
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
