
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Automated Plagiarism Checker");
    window.resize(600, 400);

    QLabel *title = new QLabel("Automated Plagiarism Checker");
    title->setAlignment(Qt::AlignCenter);

    QLabel *message = new QLabel("Welcome! Your Qt application is working.");
    message->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(title);
    layout->addWidget(message);

    window.show();

    return app.exec();
}
