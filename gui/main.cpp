#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

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
    window.resize(900, 550);

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

    QLabel *statusLabel = new QLabel("Ready - select a folder to begin");
    statusLabel->setStyleSheet(
        "color: #16a34a; font-size: 14px; font-weight: bold;"
    );

    QPushButton *checkButton = new QPushButton("Check Similarity");

    QTableWidget *table = new QTableWidget(0, 4);
    table->setHorizontalHeaderLabels(
        {"File 1", "File 2", "Similarity", "Category"}
    );

    table->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QString selectedFolder;

    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(title);
    layout->addWidget(description);
    layout->addSpacing(15);
    layout->addWidget(selectButton);
    layout->addWidget(folderLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(checkButton);
    layout->addWidget(table);

    // Select Folder button
    QObject::connect(
        selectButton, &QPushButton::clicked,
        [&window, &selectedFolder, folderLabel, table, statusLabel]()
    {
        QString folder = QFileDialog::getExistingDirectory(
            &window, "Select Folder"
        );

        if (!folder.isEmpty())
        {
            selectedFolder = folder;
            folderLabel->setText(folder);
            table->setRowCount(0);

            statusLabel->setText(
                "Folder selected - ready to compare"
            );
            statusLabel->setStyleSheet(
                "color: #2563eb; font-size: 14px; font-weight: bold;"
            );
        }
    });

    // Check Similarity button
    QObject::connect(
        checkButton, &QPushButton::clicked,
        [&window, &selectedFolder, table, statusLabel]()
    {
        if (selectedFolder.isEmpty())
        {
            statusLabel->setText("Please select a folder first");
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

        // Show comparison-in-progress status
        statusLabel->setText("Comparing files...");
        statusLabel->setStyleSheet(
            "color: #d97706; font-size: 14px; font-weight: bold;"
        );

        QApplication::processEvents();

        FileManager fileManager;

        vector<string> files =
            fileManager.findFiles(selectedFolder.toStdString());

        QMessageBox::information(
            &window,
            "Files Found",
            QString("Found %1 supported files in the selected folder.")
                .arg(files.size())
        );

        // Check whether enough files are available
        if (files.size() < 2)
        {
            statusLabel->setText("Not enough files to compare");
            statusLabel->setStyleSheet(
                "color: #dc2626; font-size: 14px; font-weight: bold;"
            );

            QMessageBox::information(
                &window, "Not Enough Files",
                "Select a folder containing at least two readable files."
            );
            return;
        }

        // Compare the files
        ComparisonManager comparison;

        vector<Result> results = comparison.compareFiles(files);

        // Check whether any results were generated
        if (results.empty())
        {
            table->setRowCount(0);

            statusLabel->setText(
                "No comparison results available"
            );
            statusLabel->setStyleSheet(
                "color: #dc2626; font-size: 14px; font-weight: bold;"
            );

            QMessageBox::warning(
                &window, "No Results",
                "No comparisons could be completed. "
                "Please check that the selected files contain readable text."
            );
            return;
        }

        // Display the results in the table
        table->setRowCount(0);

        for (const Result &result : results)
        {
            int row = table->rowCount();
            table->insertRow(row);

            QString file1 =
                QFileInfo(
                    QString::fromStdString(result.file1)
                ).fileName();

            QString file2 =
                QFileInfo(
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
        }

        // Show successful completion status
        statusLabel->setText(
            "Comparison completed successfully"
        );
        statusLabel->setStyleSheet(
            "color: #16a34a; font-size: 14px; font-weight: bold;"
        );

        QMessageBox::information(
            &window, "Comparison Complete",
            QString(
                "Compared %1 file pairs.\n"
                "Text and JSON reports have been saved."
            ).arg(results.size())
        );
    });

    window.show();

    return app.exec();
}