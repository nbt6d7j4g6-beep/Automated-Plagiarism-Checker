
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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Automated Plagiarism Checker");
    window.resize(800, 550);

    QLabel *title = new QLabel("Automated Plagiarism Checker");
    title->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #2563eb;"
    );

    QLabel *description = new QLabel(
        "Compare student files and identify textual similarity."
    );

    QPushButton *selectButton = new QPushButton("Select Folder");
    QLabel *folderLabel = new QLabel("No folder selected");

    QPushButton *checkButton = new QPushButton("Check Similarity");

    QTableWidget *table = new QTableWidget(0, 3);
    table->setHorizontalHeaderLabels(
        {"File 1", "File 2", "Similarity"}
    );
    table->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(title);
    layout->addWidget(description);
    layout->addSpacing(15);
    layout->addWidget(selectButton);
    layout->addWidget(folderLabel);
    layout->addWidget(checkButton);
    layout->addWidget(table);

    QObject::connect(selectButton, &QPushButton::clicked,
                     [&window, folderLabel]()
    {
        QString folder = QFileDialog::getExistingDirectory(
            &window, "Select Folder"
        );

        if (!folder.isEmpty())
            folderLabel->setText(folder);
    });

    QObject::connect(checkButton, &QPushButton::clicked,
                     [&window]()
    {
        QMessageBox::information(
            &window,
            "Next Step",
            "Folder selection works! Next, we will connect "
            "the comparison engine."
        );
    });

    window.show();

    return app.exec();
}
