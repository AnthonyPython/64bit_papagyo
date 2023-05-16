#ifndef BREAKDOWNDIALOG_H
#define BREAKDOWNDIALOG_H

#include <QDialog>

namespace Ui {
class BreakdownDialog;
}

class LipsyncWord;

class BreakdownDialog : public QDialog
{
	Q_OBJECT

public:
	explicit BreakdownDialog(LipsyncWord *word, QWidget *parent = 0);
	~BreakdownDialog();

	QString PhonemeString();

private slots:
	void on_aiBut();
	void on_oBut();
	void on_eBut();
	void on_uBut();
	void on_lBut();
	void on_wqBut();
	void on_mbpBut();
	void on_fvBut();
	void on_etcBut();
	void on_restBut();
    void on_chBut();
    void on_silBut();
    void on_aaBut();
    void on_aeBut();
    void on_ahBut();
    void on_aoBut();
    void on_awBut();
    void on_ayBut();
    void on_bBut();
    void on_dBut();
    void on_dhBut();
    void on_ehBut();
    void on_erBut();
    void on_eyBut();
    void on_fBut();
    void on_gBut();
    void on_hhBut();
    void on_ihBut();
    void on_iyBut();
    void on_jhBut();
    void on_kBut();
    void on_mBut();
    void on_nBut();
    void on_ngBut();
    void on_owBut();
    void on_oyBut();
    void on_pBut();
    void on_rBut();
    void on_sBut();
    void on_shBut();
    void on_tBut();
    void on_thBut();
    void on_uhBut();
    void on_uwBut();
    void on_vBut();
    void on_wBut();
    void on_yBut();
    void on_zBut();
    void on_zhBut();


private:
	void AddPhoneme(QString phoneme);

	Ui::BreakdownDialog *ui;

	LipsyncWord		*fWord;
};

#endif // BREAKDOWNDIALOG_H
