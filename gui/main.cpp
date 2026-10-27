
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
    layout->addWidget(checkButton);
    layout->addWidget(table);

    QObject::connect(selectButton, &QPushButton::clicked,
                     [&window, &selectedFolder, folderLabel, table]()
    {
        QString folder = QFileDialog::getExistingDirectory(
            &window, "Select Folder"
        );

        if (!folder.isEmpty())
        {
            selectedFolder = folder;
            folderLabel->setText(folder);
            table->setRowCount(0);
        }
    });

    QObject::connect(checkButton, &QPushButton::clicked,
                     [&window, &selectedFolder, table]()
    {
        if (selectedFolder.isEmpty())
        {
            QMessageBox::warning(
                &window, "Select Folder",
                "Please select a folder first."
            );
            return;
        }

        table->setRowCount(0);

        FileManager fileManager;
        vector<string> files =
            fileManager.findFiles(selectedFolder.toStdString());

        if (files.size() < 2)
        {
            QMessageBox::information(
                &window, "Not Enough Files",
                "Select a folder containing at least two readable files."
            );
            return;
        }

        ComparisonManager comparison;
        vector<Result> results = comparison.compareFiles(files);

        if (results.empty())
        {
                table->setRowCount(0);

                QMessageBox::warning(
                &window, "No Results",
                "No comparisons could be completed. "
                "Please check that the selected files contain readable text."
                );
            return;
        }

        table->setRowCount(0);

        for (const Result &result : results)
        {
            int row = table->rowCount();
            table->insertRow(row);

            QString file1 =
                QFileInfo(QString::fromStdString(result.file1)).fileName();
            QString file2 =
                QFileInfo(QString::fromStdString(result.file2)).fileName();

            table->setItem(row, 0, new QTableWidgetItem(file1));
            table->setItem(row, 1, new QTableWidgetItem(file2));
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

        QMessageBox::information(
            &window, "Comparison Complete",
            QString("Compared %1 file pairs.\n"
                    "Text and JSON reports have been saved.")
                .arg(results.size())
        );
    });

    window.show();

    return app.exec();
}
