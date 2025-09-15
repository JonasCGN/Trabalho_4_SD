// client Qt GUI: select a .txt file, POST its contents to master and display JSON response
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QFileInfo>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Master HTTP Client");

    QVBoxLayout *mainLayout = new QVBoxLayout(&window);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *fileLabel = new QLabel("No file selected");
    QPushButton *chooseBtn = new QPushButton("Choose .txt File");
    QPushButton *sendBtn = new QPushButton("Send to Master");
    sendBtn->setEnabled(false);

    topLayout->addWidget(chooseBtn);
    topLayout->addWidget(sendBtn);
    topLayout->addWidget(fileLabel);

    QTextEdit *responseView = new QTextEdit();
    responseView->setReadOnly(true);
    responseView->setPlaceholderText("Response from master will appear here...");

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(responseView);

    QNetworkAccessManager *manager = new QNetworkAccessManager(&window);

    QString selectedPath;

    QObject::connect(chooseBtn, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Select text file", QString(), "Text Files (*.txt);;All Files (*)");
        if (path.isEmpty()) return;
        selectedPath = path;
        QFileInfo fi(path);
        fileLabel->setText(fi.fileName());
        sendBtn->setEnabled(true);
    });

    QObject::connect(sendBtn, &QPushButton::clicked, [&]() {
        if (selectedPath.isEmpty()) {
            QMessageBox::warning(&window, "No file", "Please choose a .txt file first.");
            return;
        }

        QFile file(selectedPath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(&window, "Read error", "Failed to open file: " + selectedPath);
            return;
        }
        QByteArray data = file.readAll();
        file.close();

    QByteArray env = qgetenv("MASTER_URL");
    QString urlStr = env.isEmpty() ? QStringLiteral("http://192.168.1.12:8080/process") : QString::fromUtf8(env);
    QUrl url(urlStr);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain; charset=utf-8");

        responseView->append("--- Sending file: " + QFileInfo(selectedPath).fileName() + " ---");

        QNetworkReply *reply = manager->post(request, data);

        QObject::connect(reply, &QNetworkReply::finished, [reply, responseView, urlStr]() {
            if (reply->error() != QNetworkReply::NoError) {
                responseView->append(QString("Network error to %1: %2").arg(urlStr, reply->errorString()));
            } else {
                QByteArray resp = reply->readAll();
                responseView->append(QString("Response from %1:\n%2").arg(urlStr, QString::fromUtf8(resp)));
            }
            responseView->append("-------------------------------\n");
            reply->deleteLater();
        });
    });

    window.resize(700, 400);
    window.show();

    return app.exec();
}
