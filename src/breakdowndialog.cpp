#include "breakdowndialog.h"
#include "ui_breakdowndialog.h"
#include "lipsyncdoc.h"

BreakdownDialog::BreakdownDialog(LipsyncWord *word, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BreakdownDialog)
{
	fWord = word;

	ui->setupUi(this);
	ui->wordLabel->setText(tr("Break down the word:") + " " + fWord->fText);

	connect(ui->aiBut, SIGNAL(clicked()), this, SLOT(on_aiBut()));
	connect(ui->oBut, SIGNAL(clicked()), this, SLOT(on_oBut()));
	connect(ui->eBut, SIGNAL(clicked()), this, SLOT(on_eBut()));
	connect(ui->uBut, SIGNAL(clicked()), this, SLOT(on_uBut()));
	connect(ui->lBut, SIGNAL(clicked()), this, SLOT(on_lBut()));
	connect(ui->wqBut, SIGNAL(clicked()), this, SLOT(on_wqBut()));
	connect(ui->mbpBut, SIGNAL(clicked()), this, SLOT(on_mbpBut()));
	connect(ui->fvBut, SIGNAL(clicked()), this, SLOT(on_fvBut()));
	connect(ui->etcBut, SIGNAL(clicked()), this, SLOT(on_etcBut()));
	connect(ui->restBut, SIGNAL(clicked()), this, SLOT(on_restBut()));

    connect(ui->silBut, SIGNAL(clicked()), this, SLOT(on_silBut()));
    connect(ui->aaBut, SIGNAL(clicked()), this, SLOT(on_aaBut()));
    connect(ui->aeBut, SIGNAL(clicked()), this, SLOT(on_aeBut()));
    connect(ui->ahBut, SIGNAL(clicked()), this, SLOT(on_ahBut()));
    connect(ui->aoBut, SIGNAL(clicked()), this, SLOT(on_aoBut()));
    connect(ui->awBut, SIGNAL(clicked()), this, SLOT(on_awBut()));
    connect(ui->ayBut, SIGNAL(clicked()), this, SLOT(on_ayBut()));
    connect(ui->bBut, SIGNAL(clicked()), this, SLOT(on_bBut()));
    connect(ui->chBut, SIGNAL(clicked()), this, SLOT(on_chBut()));
    connect(ui->dBut, SIGNAL(clicked()), this, SLOT(on_dBut()));
    connect(ui->dhBut, SIGNAL(clicked()), this, SLOT(on_dhBut()));
    connect(ui->ehBut, SIGNAL(clicked()), this, SLOT(on_ehBut()));
    connect(ui->erBut, SIGNAL(clicked()), this, SLOT(on_erBut()));
    connect(ui->eyBut, SIGNAL(clicked()), this, SLOT(on_eyBut()));
    connect(ui->fBut, SIGNAL(clicked()), this, SLOT(on_fBut()));
    connect(ui->gBut, SIGNAL(clicked()), this, SLOT(on_gBut()));
    connect(ui->hhBut, SIGNAL(clicked()), this, SLOT(on_hhBut()));
    connect(ui->ihBut, SIGNAL(clicked()), this, SLOT(on_ihBut()));
    connect(ui->iyBut, SIGNAL(clicked()), this, SLOT(on_iyBut()));
    connect(ui->jhBut, SIGNAL(clicked()), this, SLOT(on_jhBut()));
    connect(ui->kBut, SIGNAL(clicked()), this, SLOT(on_kBut()));
    connect(ui->mBut, SIGNAL(clicked()), this, SLOT(on_mBut()));
    connect(ui->nBut, SIGNAL(clicked()), this, SLOT(on_nBut()));
    connect(ui->ngBut, SIGNAL(clicked()), this, SLOT(on_ngBut()));
    connect(ui->owBut, SIGNAL(clicked()), this, SLOT(on_owBut()));
    connect(ui->oyBut, SIGNAL(clicked()), this, SLOT(on_oyBut()));
    connect(ui->pBut, SIGNAL(clicked()), this, SLOT(on_pBut()));
    connect(ui->rBut, SIGNAL(clicked()), this, SLOT(on_rBut()));
    connect(ui->sBut, SIGNAL(clicked()), this, SLOT(on_sBut()));
    connect(ui->shBut, SIGNAL(clicked()), this, SLOT(on_shBut()));
    connect(ui->tBut, SIGNAL(clicked()), this, SLOT(on_tBut()));
    connect(ui->thBut, SIGNAL(clicked()), this, SLOT(on_thBut()));
    connect(ui->uhBut, SIGNAL(clicked()), this, SLOT(on_uhBut()));
    connect(ui->uwBut, SIGNAL(clicked()), this, SLOT(on_uwBut()));
    connect(ui->vBut, SIGNAL(clicked()), this, SLOT(on_vBut()));
    connect(ui->wBut, SIGNAL(clicked()), this, SLOT(on_wBut()));
    connect(ui->yBut, SIGNAL(clicked()), this, SLOT(on_yBut()));
    connect(ui->zBut, SIGNAL(clicked()), this, SLOT(on_zBut()));
    connect(ui->zhBut, SIGNAL(clicked()), this, SLOT(on_zhBut()));


	QString str;
	for (int i = 0; i < fWord->fPhonemes.size(); i++)
	{
		str += " ";
		str += fWord->fPhonemes[i]->fText;
	}
	ui->breakdownEdit->setText(str.trimmed());
}

BreakdownDialog::~BreakdownDialog()
{
	delete ui;
}

QString BreakdownDialog::PhonemeString()
{
	return ui->breakdownEdit->text().trimmed();
}

void BreakdownDialog::on_aiBut()
{
	AddPhoneme("AI");
}

void BreakdownDialog::on_oBut()
{
	AddPhoneme("O");
}

void BreakdownDialog::on_eBut()
{
	AddPhoneme("E");
}

void BreakdownDialog::on_uBut()
{
	AddPhoneme("U");
}

void BreakdownDialog::on_lBut()
{
	AddPhoneme("L");
}

void BreakdownDialog::on_wqBut()
{
	AddPhoneme("WQ");
}

void BreakdownDialog::on_mbpBut()
{
	AddPhoneme("MBP");
}

void BreakdownDialog::on_fvBut()
{
	AddPhoneme("FV");
}

void BreakdownDialog::on_etcBut()
{
	AddPhoneme("etc");
}

void BreakdownDialog::on_restBut()
{
	AddPhoneme("rest");
}

void BreakdownDialog::on_chBut()
{
    AddPhoneme("CH");
}

void BreakdownDialog::on_silBut()
{
    AddPhoneme("SIL");
}

void BreakdownDialog::on_aaBut()
{
    AddPhoneme("AA");
}

void BreakdownDialog::on_aeBut()
{
    AddPhoneme("AE");
}

void BreakdownDialog::on_ahBut()
{
    AddPhoneme("AH");
}

void BreakdownDialog::on_aoBut()
{
    AddPhoneme("AO");
}

void BreakdownDialog::on_awBut()
{
    AddPhoneme("AW");
}

void BreakdownDialog::on_ayBut()
{
    AddPhoneme("AY");
}

void BreakdownDialog::on_bBut()
{
    AddPhoneme("B");
}

void BreakdownDialog::on_dBut()
{
    AddPhoneme("D");
}

void BreakdownDialog::on_dhBut()
{
    AddPhoneme("DH");
}

void BreakdownDialog::on_ehBut()
{
    AddPhoneme("EH");
}

void BreakdownDialog::on_erBut()
{
    AddPhoneme("ER");
}

void BreakdownDialog::on_eyBut()
{
    AddPhoneme("EY");
}

void BreakdownDialog::on_fBut()
{
    AddPhoneme("F");
}

void BreakdownDialog::on_gBut()
{
    AddPhoneme("G");
}

void BreakdownDialog::on_hhBut()
{
    AddPhoneme("HH");
}

void BreakdownDialog::on_ihBut()
{
    AddPhoneme("IH");
}

void BreakdownDialog::on_iyBut()
{
    AddPhoneme("IY");
}

void BreakdownDialog::on_jhBut()
{
    AddPhoneme("JH");
}

void BreakdownDialog::on_kBut()
{
    AddPhoneme("K");
}

void BreakdownDialog::on_mBut()
{
    AddPhoneme("M");
}

void BreakdownDialog::on_nBut()
{
    AddPhoneme("N");
}

void BreakdownDialog::on_ngBut()
{
    AddPhoneme("NG");
}

void BreakdownDialog::on_owBut()
{
    AddPhoneme("OW");
}

void BreakdownDialog::on_oyBut()
{
    AddPhoneme("OY");
}

void BreakdownDialog::on_pBut()
{
    AddPhoneme("P");
}

void BreakdownDialog::on_rBut()
{
    AddPhoneme("R");
}

void BreakdownDialog::on_sBut()
{
    AddPhoneme("S");
}

void BreakdownDialog::on_shBut()
{
    AddPhoneme("SH");
}

void BreakdownDialog::on_tBut()
{
    AddPhoneme("T");
}

void BreakdownDialog::on_thBut()
{
    AddPhoneme("TH");
}

void BreakdownDialog::on_uhBut()
{
    AddPhoneme("UH");
}

void BreakdownDialog::on_uwBut()
{
    AddPhoneme("UW");
}

void BreakdownDialog::on_vBut()
{
    AddPhoneme("V");
}

void BreakdownDialog::on_wBut()
{
    AddPhoneme("W");
}

void BreakdownDialog::on_yBut()
{
    AddPhoneme("Y");
}

void BreakdownDialog::on_zBut()
{
    AddPhoneme("Z");
}

void BreakdownDialog::on_zhBut()
{
    AddPhoneme("ZH");
}

void BreakdownDialog::AddPhoneme(QString phoneme)
{
	QString str = ui->breakdownEdit->text().trimmed();
	str += " ";
	str += phoneme;
	ui->breakdownEdit->setText(str.trimmed());
}

