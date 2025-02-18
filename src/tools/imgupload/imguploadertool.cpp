// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2017-2019 Alejandro Sirgo Rica & Contributors

#include "imguploadertool.h"
#include "imguploadermanager.h"
#include <QInputDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTextEdit>

ImgUploaderTool::ImgUploaderTool(QObject* parent)
  : AbstractActionTool(parent)
{
    auto m_formatComboBox = new QComboBox();
    m_formatComboBox->addItem("Markdown");
    m_formatComboBox->addItem("BBCode");
    m_formatComboBox->addItem("HTML");
    m_formatComboBox->addItem("URL");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_formatComboBox);
}

bool ImgUploaderTool::closeOnButtonPressed() const
{
    return true;
}

QIcon ImgUploaderTool::icon(const QColor& background, bool inEditor) const
{
    Q_UNUSED(inEditor);
    return QIcon(iconPath(background) + "cloud-upload.svg");
}

QString ImgUploaderTool::name() const
{
    return tr("Image Uploader");
}

CaptureTool::Type ImgUploaderTool::type() const
{
    return CaptureTool::TYPE_IMAGEUPLOADER;
}

QString ImgUploaderTool::description() const
{
    return tr("Upload the selection");
}

CaptureTool* ImgUploaderTool::copy(QObject* parent)
{
    return new ImgUploaderTool(parent);
}

void ImgUploaderTool::pressed(CaptureContext& context)
{
    emit requestAction(REQ_CLEAR_SELECTION);
    emit requestAction(REQ_CAPTURE_DONE_OK);
    context.request.addTask(CaptureRequest::UPLOAD);
    emit requestAction(REQ_CLOSE_GUI);

    bool ok;
    QStringList formats;
    formats << "markdown"
            << "bbcode"
            << "html"
            << "url";
    QString selectedFormat =
      QInputDialog::getItem(nullptr,
                            "Select Image Format",
                            "Choose format for image URL:",
                            formats,
                            0,
                            false,
                            &ok);

    if (ok && !selectedFormat.isEmpty()) {
        ImgUploaderManager* uploaderManager = new ImgUploaderManager();
        QString imageUrl =
          uploaderManager
            ->url();
        QString imageCode =
          uploaderManager->generateImageCode(selectedFormat, imageUrl);

        QMessageBox msgBox;
        msgBox.setWindowTitle("Generated Image Code");

        auto* textEdit = new QTextEdit(&msgBox);
        textEdit->setText(imageCode); 
        textEdit->setReadOnly(
          true); 
        textEdit->setPlainText(imageCode); 

        msgBox.layout()->addWidget(textEdit);
        msgBox.exec();
    }
}
