#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    typedef struct {
        QTextCharFormat format;
        QRegExp expression;
    } SyntaxHighlightingRule;

protected:
    QVector<SyntaxHighlightingRule> m_syntaxHighlightingRules;

public:
    explicit SyntaxHighlighter(QTextDocument *parent);

    void registerRule(const SyntaxHighlightingRule &rule);
    void registerRule(const QTextCharFormat &format, const QRegExp &expression);
    void registerRule(const QTextCharFormat &format, const QString &pattern);

    void registerRules(const QTextCharFormat &format, const QList<QRegExp> &expressions);
    void registerRules(const QTextCharFormat &format, const QStringList &patterns);

    void clearRules();

protected:
    void highlightBlock(const QString &text) override;
    void highlightBlock(const QString &text, const SyntaxHighlightingRule &rule);

signals:

public slots:
};

#endif // SYNTAXHIGHLIGHTER_H
