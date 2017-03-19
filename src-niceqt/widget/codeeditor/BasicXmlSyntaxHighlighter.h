#ifndef BASICXMLSYNTAXHIGHLIGHTER_H
#define BASICXMLSYNTAXHIGHLIGHTER_H

#include "SyntaxHighlighter.h"

/**
 * The code in this class is highly inspired by
 *     https://github.com/d1vanov/basic-xml-syntax-highlighter
 */
class BasicXmlSyntaxHighlighter : public SyntaxHighlighter
{
    Q_OBJECT

public:
    explicit BasicXmlSyntaxHighlighter(QTextDocument *parent);

protected:
    void highlightBlock(const QString &text) override;

signals:

public slots:
};

#endif // BASICXMLSYNTAXHIGHLIGHTER_H
