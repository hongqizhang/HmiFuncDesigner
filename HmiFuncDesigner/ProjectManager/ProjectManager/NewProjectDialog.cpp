﻿#include "NewProjectDialog.h"
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QSettings>
#include "ConfigUtils.h"
#include "DrawListUtils.h"
#include "ProjectData.h"
#include "ProjectInfoManager.h"
#include "Singleton.h"
#include "ui_NewProjectDialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent, QString projPath)
    : QDialog(parent),
      ui(new Ui::NewProjectDialog),
      projectPath_(projPath)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    ui->editProjectName->setText("demo1");
    ui->editProjectDescription->setText("demo1 project");
    ui->editStationNumber->setText("0");
    ui->editStationAddress->setText("192.168.1.10");

    ui->cboStartPage->clear();
    DrawListUtils::loadDrawList(projPath);
    for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
        QString strPageName = DrawListUtils::drawList_.at(i) + ".drw";
        ui->cboStartPage->addItem(strPageName);
    }
    ui->cboStartPage->addItem("None");

    ui->editPageScanPeriod->setText("500");
    ui->editDataScanPeriod->setText("500");

    /////////////////////////////////////////////////////

    QString iniDevFileName = QCoreApplication::applicationDirPath() + "/Config/device.ini";
    QFile fileDevCfg(iniDevFileName);
    if (!fileDevCfg.exists())
        QMessageBox::critical(this,
                              tr("提示"),
                              tr("设备选型配置文件不存在，您不能创建新工程！"));

    QSettings settingsDev(iniDevFileName, QSettings::IniFormat);
    QStringList slistDev;
    ConfigUtils::getCfgList(iniDevFileName, "DeviceSupportIndex", "dev", slistDev);

    settingsDev.beginGroup("DeviceID");
    for (int i = 0; i < slistDev.count(); i++) {
        int value = settingsDev.value(slistDev.at(i)).toInt();
        deviceMap_.insert(slistDev.at(i), value);
        ui->cboDevType->insertItem(i, slistDev.at(i));
    }
    settingsDev.endGroup();
}

NewProjectDialog::~NewProjectDialog() { delete ui; }

void NewProjectDialog::on_btnFileDialog_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("选择工程目录"),
                                                    "C:/",
                                                    QFileDialog::ShowDirsOnly);
    ui->editProjectPath->setText(dir);
}

void NewProjectDialog::on_btnHelp_clicked() {
    // this time we do nothing!
}

bool NewProjectDialog::check_data() {
    bool ret = true;

    QDir dir(ui->editProjectPath->text());
    if (!dir.exists() || ui->editProjectPath->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("输入路径错误！"));
        ret = false;
    }

    if (ui->editProjectName->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("工程名称输入错误！"));
        ret = false;
    }

    if (ui->editStationNumber->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("本站站号不能为空！"));
        ret = false;
    }

    if (ui->editStationAddress->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("本站地址不能为空！"));
        ret = false;
    }

    if (ui->editPageScanPeriod->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("画面刷新率不能为空！"));
        ret = false;
    }

    if (ui->editDataScanPeriod->text().isEmpty()) {
        QMessageBox::information(this, tr("系统提示"), tr("数据刷新率不能为空！"));
        ret = false;
    }

    return ret;
}

void NewProjectDialog::on_btnCheck_clicked() {
    if (check_data()) {
        QMessageBox::information(this, tr("系统提示"), tr("设置正确！"));
    }
}

void NewProjectDialog::on_btnOk_clicked() {
    if (check_data()) {
        QString strProjPath = ui->editProjectPath->text();
        QString strProjName = ui->editProjectName->text();
        QString file = strProjPath + "/" + strProjName + ".pdt";
        projectName_ = file;
        QDialog::accept();
    }
}

void NewProjectDialog::on_btnExit_clicked() {
    QDialog::reject();
}

QString NewProjectDialog::GetProjectName() {
    return projectName_;
}

bool NewProjectDialog::load() {
    ProjectInfoManager &projInfoMgr = ProjectData::getInstance()->projInfoMgr_;
    projInfoMgr.load(ProjectData::getInstance()->dbData_);
    ui->editProjectName->setText(projInfoMgr.getProjectName());
    ui->editProjectDescription->setPlainText(projInfoMgr.getProjectDescription());
    ui->cboDevType->setCurrentText(projInfoMgr.getDeviceType());
    ui->editStationNumber->setText(QString::number(projInfoMgr.getStationNumber()));
    ui->cboStartPage->setCurrentText(projInfoMgr.getStartPage());
    ui->editStationAddress->setText(projInfoMgr.getStationAddress());
    ui->chkProjectEncrypt->setChecked(projInfoMgr.getProjectEncrypt());
    ui->editPageScanPeriod->setText(QString::number(projInfoMgr.getPageScanPeriod()));
    ui->editDataScanPeriod->setText(QString::number(projInfoMgr.getDataScanPeriod()));
    ui->editProjectPath->setText(projectPath_);
    return true;
}

bool NewProjectDialog::save() {
    ProjectInfoManager &projInfoMgr = ProjectData::getInstance()->projInfoMgr_;
    projInfoMgr.setProjectName(ui->editProjectName->text());
    projInfoMgr.setProjectDescription(ui->editProjectDescription->toPlainText());
    projInfoMgr.setProjectPath(ui->editProjectPath->text());
    projInfoMgr.setDeviceType(ui->cboDevType->currentText());
    projInfoMgr.setStationNumber(ui->editStationNumber->text().toInt());
    projInfoMgr.setStartPage(ui->cboStartPage->currentText());
    projInfoMgr.setStationAddress(ui->editStationAddress->text());
    projInfoMgr.setProjectEncrypt(ui->chkProjectEncrypt->isChecked());
    projInfoMgr.setPageScanPeriod(ui->editPageScanPeriod->text().toInt());
    projInfoMgr.setDataScanPeriod(ui->editDataScanPeriod->text().toInt());
    projInfoMgr.save(ProjectData::getInstance()->dbData_);
    return true;
}
