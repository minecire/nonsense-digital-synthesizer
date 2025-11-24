
#include "LookAndFeel.h"


const juce::Colour NonsenseLookAndFeel::backgroundColor1 = juce::Colour::fromRGB(250, 220, 210);
const juce::Colour NonsenseLookAndFeel::backgroundColor2 = juce::Colour::fromRGB(230, 200, 180);
const juce::Colour NonsenseLookAndFeel::backgroundColor3 = juce::Colour::fromRGB(240, 180, 150);
const juce::Colour NonsenseLookAndFeel::penColor = juce::Colour::fromRGB(20, 45, 105);
const juce::Colour NonsenseLookAndFeel::fancyPenColors[10] = {
    juce::Colour::fromRGB(105, 45, 20),
    juce::Colour::fromRGB(95, 65, 20),
    juce::Colour::fromRGB(85, 85, 25),
    juce::Colour::fromRGB(20, 105, 45),
    juce::Colour::fromRGB(20, 85, 85),
    juce::Colour::fromRGB(20, 45, 105),
    juce::Colour::fromRGB(45, 20, 95),
    juce::Colour::fromRGB(85, 20, 85),
    juce::Colour::fromRGB(100, 20, 45),
    juce::Colour::fromRGB(85, 85, 85)
};
NonsenseLookAndFeel::NonsenseLookAndFeel() {
}
void NonsenseLookAndFeel::drawScrollbar(juce::Graphics& g, juce::ScrollBar& scrollbar, int x, int y, int width, int height,
    bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, [[maybe_unused]] bool isMouseDown)
{
    juce::Rectangle<int> thumbBounds;

    if (isScrollbarVertical)
        thumbBounds = { x, thumbStartPosition, width, thumbSize };
    else
        thumbBounds = { thumbStartPosition, y, thumbSize, height };

    auto c = scrollbar.findColour(juce::ScrollBar::ColourIds::thumbColourId);
    g.setColour(c);
    fillRectWithLines(g, x, y, width, height, 6.0);

    g.setColour(backgroundColor1);
    g.fillRect(thumbBounds.reduced(1).toFloat());
    g.setColour(c);
    g.drawRect(thumbBounds.reduced(1).toFloat());
}

void NonsenseLookAndFeel::fillRectWithLines(juce::Graphics& g, int x, int y, int w, int h, float spacing) {
    for (int i = x; i < x + w - h; i += spacing) {
        g.drawLine(i, y, i + h, y + h);
    }
    if (w > h) {
        for (int i = x + w - h; i < x + w; i += spacing) {
            g.drawLine(i, y, x + w, y + w - i);
        }
    }
    else {
        for (int i = x; i < x + w; i += spacing) {
            g.drawLine(i, y, x + w, y + w - i);
        }
    }
    for (int i = y + spacing; i < y + h - w; i += spacing) {
        g.drawLine(x, i, x + w, i + w - x);
    }
    if (h > w) {
        for (int i = y + h - w; i < y + h; i += spacing) {
            g.drawLine(x, i, x + h - i, y + h);
        }
    }
    else {
        for (int i = y + spacing; i < y + h; i += spacing) {
            g.drawLine(x, i, x + h - i, y + h);
        }
    }
}

void NonsenseLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool,
    int a, int b, int c, int d, juce::ComboBox& box)
{
    auto cornerSize = box.findParentComponentOfClass<juce::ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
    juce::Rectangle<int> boxBounds(0, 0, width, height);

    //g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
    //g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

    g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
    g.drawRect(boxBounds.toFloat(), 2.0f);

    juce::Rectangle<int> arrowZone(width - 30, 0, 20, height);
    juce::Path path;
    path.startNewSubPath((float)arrowZone.getX() + 3.0f, (float)arrowZone.getCentreY() - 1.0f);
    path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 4.0f);
    path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() - 4.0f);
    path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 4.0f);
    path.lineTo((float)arrowZone.getRight() - 3.0f, (float)arrowZone.getCentreY() - 1.0f);

    g.strokePath(path, juce::PathStrokeType(2.0f));
}

void NonsenseLookAndFeel::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
    const bool isSeparator, const bool isActive,
    const bool isHighlighted, const bool isTicked,
    const bool hasSubMenu, const juce::String& text,
    const juce::String& shortcutKeyText,
    const juce::Drawable* icon, const juce::Colour* const textColourToUse)
{
    if (isSeparator)
    {
        auto r = area.reduced(5, 0);
        r.removeFromTop(juce::roundToInt(((float)r.getHeight() * 0.5f) - 0.5f));

        g.setColour(penColor);
        g.fillRect(r.removeFromTop(1));
    }
    else
    {

        auto r = area.reduced(1);

        if (isHighlighted && isActive)
        {
            g.setColour(backgroundColor1);

        }
        else
        {
            g.setColour(backgroundColor3);
        }
        g.fillRect(r);
        g.setColour(penColor);

        r.reduce(juce::jmin(5, area.getWidth() / 20), 0);

        auto font = getPopupMenuFont();

        auto maxFontHeight = (float)r.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight(maxFontHeight);

        g.setFont(font);

        auto iconArea = r.removeFromLeft(juce::roundToInt(maxFontHeight)).toFloat();

        if (icon != nullptr)
        {
            icon->drawWithin(g, iconArea, juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
            r.removeFromLeft(juce::roundToInt(maxFontHeight * 0.5f));
        }
        else if (isTicked)
        {
            auto tick = getTickShape(1.0f);
            g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
        }

        if (hasSubMenu)
        {
            auto arrowH = 0.6f * getPopupMenuFont().getAscent();

            auto x = static_cast<float> (r.removeFromRight((int)arrowH).getX());
            auto halfH = static_cast<float> (r.getCentreY());

            juce::Path path;
            path.startNewSubPath(x, halfH - arrowH * 0.5f);
            path.lineTo(x + arrowH * 0.6f, halfH);
            path.lineTo(x, halfH + arrowH * 0.5f);

            g.strokePath(path, juce::PathStrokeType(2.0f));
        }

        r.removeFromRight(3);
        g.drawFittedText(text, r, juce::Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            auto f2 = font;
            f2.setHeight(f2.getHeight() * 0.75f);
            f2.setHorizontalScale(0.95f);
            g.setFont(f2);

            g.drawText(shortcutKeyText, r, juce::Justification::centredRight, true);
        }
    }
}

void NonsenseLookAndFeel::drawButtonBackground(juce::Graphics& g,
    juce::Button& button,
    const juce::Colour& backgroundColour,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
    float x = bounds.getX();
    float y = bounds.getY();
    float w = bounds.getWidth();
    float h = bounds.getHeight();
    if (shouldDrawButtonAsDown) {
        juce::Rectangle<float> tickBounds(x + 1.0, y + 9.0, w - 2.0, h - 10.0);
        g.setColour(backgroundColour);
        g.drawRect(tickBounds);
    }
    else if (button.getToggleState()) {
        juce::Rectangle<float> tickBounds(x + 1.0, y + 6.0, w - 2.0, h - 10.0);
        g.setColour(backgroundColour);
        g.drawRect(tickBounds);
        for (int i = x + 2.0; i < x + w - 1.0; i += 3) {
            g.drawLine(i, y + h - 5.0, i, y + h - 2.0);
        }
    }
    else {
        juce::Rectangle<float> tickBounds(x + 1.0, y + 1.0, w - 2.0, h - 10.0);
        g.setColour(backgroundColour);
        g.drawRect(tickBounds);
        for (int i = x + 2.0; i < x + w - 1.0; i += 3) {
            g.drawLine(i, y + h - 10.0, i, y + h - 2.0);
        }
    }
}
void NonsenseLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto fontSize = juce::jmin(15.0f, (float)button.getHeight() * 0.75f);
    auto tickWidth = fontSize * 1.1f;

    drawTickBox(g, button, 4.0f, ((float)button.getHeight() - tickWidth) * 0.5f,
        tickWidth, tickWidth,
        button.getToggleState(),
        button.isEnabled(),
        shouldDrawButtonAsHighlighted,
        shouldDrawButtonAsDown);

    g.setColour(button.findColour(juce::ToggleButton::textColourId));
    g.setFont(fontSize);

    if (!button.isEnabled())
        g.setOpacity(0.5f);

    g.drawFittedText(button.getButtonText(),
        button.getLocalBounds().withTrimmedLeft(juce::roundToInt(tickWidth) + 10)
        .withTrimmedRight(2),
        juce::Justification::centredLeft, 10);
}
void NonsenseLookAndFeel::drawTickBox(juce::Graphics& g, juce::Component& component,
    float x, float y, float w, float h,
    const bool ticked,
    [[maybe_unused]] const bool isEnabled,
    [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
    [[maybe_unused]] const bool shouldDrawButtonAsDown)
{
    if (ticked) {
        juce::Rectangle<float> tickBounds(x, y, w, h);
        g.setColour(component.findColour(juce::ToggleButton::tickDisabledColourId));
        g.drawRect(tickBounds);
    }
    else {
        juce::Rectangle<float> tickBounds(x, y - 8.0, w, h);
        g.setColour(component.findColour(juce::ToggleButton::tickDisabledColourId));
        g.drawRect(tickBounds);
        for (int i = x; i < x + w; i += 2) {
            g.drawLine(i, y + h - 8.0, i, y + h);
        }
    }

}
void NonsenseLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos,
    float minSliderPos,
    float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    if (slider.isBar())
    {
        g.setColour(slider.findColour(juce::Slider::trackColourId));
        g.fillRect(slider.isHorizontal() ? juce::Rectangle<float>(static_cast<float> (x), (float)y + 0.5f, sliderPos - (float)x, (float)height - 1.0f)
            : juce::Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f, (float)y + ((float)height - sliderPos)));

        drawLinearSliderOutline(g, x, y, width, height, style, slider);
    }
    else
    {
        auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueVertical || style == juce::Slider::SliderStyle::TwoValueHorizontal);
        auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical || style == juce::Slider::SliderStyle::ThreeValueHorizontal);

        auto trackWidth = juce::jmin(6.0f, slider.isHorizontal() ? (float)height * 0.25f : (float)width * 0.25f);

        juce::Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
            slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

        juce::Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
            slider.isHorizontal() ? startPoint.y : (float)y);

        g.setColour(slider.findColour(juce::Slider::backgroundColourId));
        //g.strokePath(backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        if (slider.isHorizontal()) {
            for (float i = startPoint.x + trackWidth / 2.0; i < endPoint.x - trackWidth / 2.0; i += 4.0)
            {
                juce::Line<float> line(i, startPoint.y - trackWidth / 2.0, i + trackWidth, startPoint.y + trackWidth / 2.0);
                g.drawLine(line);
                juce::Line<float> line2(i, startPoint.y - trackWidth / 2.0 - 2.0, i, startPoint.y - trackWidth / 2.0);
                g.drawLine(line2);
                juce::Line<float> line3(i + 2.0, startPoint.y - trackWidth / 2.0 - 2.0, i + 2.0, startPoint.y - trackWidth / 2.0);
                g.drawLine(line3);
            }
            juce::Line<float> top(startPoint.x, startPoint.y - trackWidth / 2.0 - 2.0, endPoint.x, startPoint.y - trackWidth / 2.0 - 2.0);
            juce::Line<float> bottom(startPoint.x, startPoint.y + trackWidth / 2.0, endPoint.x, startPoint.y + trackWidth / 2.0);
            g.drawLine(top);
            g.drawLine(bottom);
        }
        else {
            for (float i = startPoint.y - trackWidth / 2.0; i > endPoint.y; i -= 4.0)
            {
                juce::Line<float> line(startPoint.x - trackWidth / 2.0, i - trackWidth / 2.0, startPoint.x + trackWidth / 2.0, i + trackWidth / 2.0);
                g.drawLine(line);
            }
            juce::Line<float> left(startPoint.x - trackWidth / 2.0, startPoint.y, endPoint.x - trackWidth / 2.0, endPoint.y - 2.0);
            juce::Line<float> right(startPoint.x + trackWidth / 2.0, startPoint.y, endPoint.x + trackWidth / 2.0, endPoint.y - 2.0);
            g.drawLine(left);
            g.drawLine(right);
            for (float i = endPoint.x - trackWidth / 2.0; i < endPoint.x + trackWidth / 2.0; i += 2.0)
            {
                juce::Line<float> line(i, endPoint.y, i, endPoint.y - 2.0);
                g.drawLine(line);
            }
        }

        juce::Path valueTrack;
        juce::Point<float> minPoint, maxPoint, thumbPoint;

        if (isTwoVal || isThreeVal)
        {
            minPoint = { slider.isHorizontal() ? minSliderPos : (float)width * 0.5f,
                         slider.isHorizontal() ? (float)height * 0.5f : minSliderPos };

            if (isThreeVal)
                thumbPoint = { slider.isHorizontal() ? sliderPos : (float)width * 0.5f,
                               slider.isHorizontal() ? (float)height * 0.5f : sliderPos };

            maxPoint = { slider.isHorizontal() ? maxSliderPos : (float)width * 0.5f,
                         slider.isHorizontal() ? (float)height * 0.5f : maxSliderPos };
        }
        else
        {
            auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
            auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

            minPoint = startPoint;
            maxPoint = { kx, ky };
        }

        auto thumbWidth = 10.0;// getSliderThumbRadius(slider);

        /*valueTrack.startNewSubPath(minPoint);
        valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
        g.setColour(slider.findColour(juce::Slider::trackColourId));
        g.strokePath(valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });*/

        if (slider.isHorizontal()) {
            for (float i = startPoint.x; i < maxPoint.x; i += 4.0)
            {
                juce::Line<float> line(i + trackWidth, startPoint.y - trackWidth / 2.0, i, startPoint.y + trackWidth / 2.0);
                g.drawLine(line);
            }
        }
        else {
            for (float i = startPoint.y - trackWidth / 2.0; i > maxPoint.y; i -= 4.0)
            {
                juce::Line<float> line(startPoint.x - trackWidth / 2.0, i + trackWidth / 2.0, startPoint.x + trackWidth / 2.0, i - trackWidth / 2.0);
                g.drawLine(line);
            }
        }

        if (!isTwoVal)
        {
            g.setColour(backgroundColor2);
            juce::Rectangle<float> rect(maxPoint.x - thumbWidth * (slider.isHorizontal() ? 0.4 : 1.0), 
                maxPoint.y - thumbWidth * (slider.isHorizontal() ? 1.0 : 0.4) - (slider.isHorizontal() ? 2.0 : 0.0) - 2.0, 
                thumbWidth* (slider.isHorizontal() ? 0.8 : 2.0), thumbWidth* (slider.isHorizontal() ? 2.0 : 0.8) + 4.0);
            g.fillRect(rect);
            g.setColour(slider.findColour(juce::Slider::backgroundColourId));
            g.drawRect(rect);
            for (float i = maxPoint.x - thumbWidth * (slider.isHorizontal() ? 0.4 : 1.0); 
                i < maxPoint.x - thumbWidth * (slider.isHorizontal() ? 0.4 : 1.0) + thumbWidth * (slider.isHorizontal() ? 0.8 : 2.0); i+=2)
            {
                g.drawLine(i, maxPoint.y + thumbWidth * (slider.isHorizontal() ? 1.0 : 0.4) - (slider.isHorizontal() ? 2.0 : 0.0) - 2.0, 
                    i, maxPoint.y + thumbWidth * (slider.isHorizontal() ? 1.0 : 0.4) - (slider.isHorizontal() ? 2.0 : 0.0) + 2.0);
            }
        }

        if (isTwoVal || isThreeVal)
        {
            auto sr = juce::jmin(trackWidth, (slider.isHorizontal() ? (float)height : (float)width) * 0.4f);
            auto pointerColour = slider.findColour(juce::Slider::thumbColourId);

            if (slider.isHorizontal())
            {
                drawPointer(g, minSliderPos - sr,
                    juce::jmax(0.0f, (float)y + (float)height * 0.5f - trackWidth * 2.0f),
                    trackWidth * 2.0f, pointerColour, 2);

                drawPointer(g, maxSliderPos - trackWidth,
                    juce::jmin((float)(y + height) - trackWidth * 2.0f, (float)y + (float)height * 0.5f),
                    trackWidth * 2.0f, pointerColour, 4);
            }
            else
            {
                drawPointer(g, juce::jmax(0.0f, (float)x + (float)width * 0.5f - trackWidth * 2.0f),
                    minSliderPos - trackWidth,
                    trackWidth * 2.0f, pointerColour, 1);

                drawPointer(g, juce::jmin((float)(x + width) - trackWidth * 2.0f, (float)x + (float)width * 0.5f), maxSliderPos - sr,
                    trackWidth * 2.0f, pointerColour, 3);
            }
        }

        if (slider.isBar())
            drawLinearSliderOutline(g, x, y, width, height, style, slider);
    }
}



void NonsenseLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto outline = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
    auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);

    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);

    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = juce::jmin(8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;

    fillCircleArcWithLines(bounds.getCentreX(), bounds.getCentreY() - 5.0, arcRadius - lineW * 0.5, arcRadius + lineW * 0.5, slider.findColour(juce::Slider::backgroundColourId), 4.0, toAngle + 0.6, g);

    fillCircleArcWithLines(bounds.getCentreX(), bounds.getCentreY() - 5.0, 0.0, arcRadius - lineW * 0.5, slider.findColour(juce::Slider::backgroundColourId), 2.0, toAngle + juce::MathConstants<float>::halfPi + 0.2, g);

    g.setColour(slider.findColour(juce::Slider::backgroundColourId));

    juce::Rectangle<float> circleRect(bounds.getCentreX() - arcRadius - lineW * 0.5, bounds.getCentreY() - arcRadius - lineW * 0.5 - 5.0,
        (arcRadius + lineW * 0.5) * 2.0, (arcRadius + lineW * 0.5) * 2.0);

    g.drawEllipse(circleRect, 1.0);

    for (float i = toAngle; i < toAngle + juce::MathConstants<float>::twoPi; i += 0.3)
    {
        if (i > juce::MathConstants<float>::pi * 3.0 && i < juce::MathConstants<float>::pi * 4.0 || i < juce::MathConstants<float>::pi * 2.0) {
            continue;
        }
        juce::Line<float> line(bounds.getCentreX() + (arcRadius + lineW * 0.5) * cos(i), bounds.getCentreY() + (arcRadius + lineW * 0.5) * sin(i) - 5.0,
            bounds.getCentreX() + (arcRadius + lineW * 0.5) * cos(i), bounds.getCentreY() + (arcRadius + lineW * 0.5) * sin(i) + 5.0);
        g.drawLine(line);
    }

    float pointAngle = toAngle - juce::MathConstants<float>::halfPi;

    if (pointAngle > juce::MathConstants<float>::pi * 2.0 || pointAngle < juce::MathConstants<float>::pi) {
        juce::Line<float> line(bounds.getCentreX() + (arcRadius + lineW * 0.5) * cos(pointAngle), bounds.getCentreY() + (arcRadius + lineW * 0.5) * sin(pointAngle) - 5.0,
            bounds.getCentreX() + (arcRadius + lineW * 0.5) * cos(pointAngle), bounds.getCentreY() + (arcRadius + lineW * 0.5) * sin(pointAngle) + 5.0);
        g.drawLine(line, 3.0);
    }

    auto thumbWidth = lineW * 2.0f;
    juce::Line<float> pointerLine(bounds.getCentreX() + (arcRadius - lineW * 0.45) * std::cos(toAngle - juce::MathConstants<float>::halfPi),
        bounds.getCentreY() + (arcRadius - lineW * 0.45) * std::sin(toAngle - juce::MathConstants<float>::halfPi) - 5.0,
        bounds.getCentreX() + (arcRadius + lineW * 0.45) * std::cos(toAngle - juce::MathConstants<float>::halfPi),
        bounds.getCentreY() + (arcRadius + lineW * 0.45) * std::sin(toAngle - juce::MathConstants<float>::halfPi) - 5.0
    );
    g.drawLine(pointerLine, 3.0);

    juce::Line<float> startLine(bounds.getCentreX() + (arcRadius + lineW * 0.85) * std::cos(rotaryStartAngle - juce::MathConstants<float>::halfPi),
        bounds.getCentreY() + (arcRadius + lineW * 0.85) * std::sin(rotaryStartAngle - juce::MathConstants<float>::halfPi) + 5.0,
        bounds.getCentreX() + (arcRadius + lineW * 1.6) * std::cos(rotaryStartAngle - juce::MathConstants<float>::halfPi),
        bounds.getCentreY() + (arcRadius + lineW * 1.6) * std::sin(rotaryStartAngle - juce::MathConstants<float>::halfPi) + 5.0
    );
    g.drawLine(startLine, 3.0);

    juce::Line<float> endLine(bounds.getCentreX() + (arcRadius + lineW * 0.85) * std::cos(rotaryEndAngle - juce::MathConstants<float>::halfPi),
        bounds.getCentreY() + (arcRadius + lineW * 0.85) * std::sin(rotaryEndAngle - juce::MathConstants<float>::halfPi) + 5.0,
        bounds.getCentreX() + (arcRadius + lineW * 1.6) * std::cos(rotaryEndAngle - juce::MathConstants<float>::halfPi),
        bounds.getCentreY() + (arcRadius + lineW * 1.6) * std::sin(rotaryEndAngle - juce::MathConstants<float>::halfPi) + 5.0
    );
    g.drawLine(endLine, 3.0);


}



void NonsenseLookAndFeel::fillCircleArcWithLines(float centerX, float centerY, float rInner, float rOuter, juce::Colour lineColor, float lineSpacing, float linesAngle, juce::Graphics& g)
{
    g.setColour(lineColor);

    while (linesAngle > juce::MathConstants<float>::pi) {
        linesAngle -= juce::MathConstants<float>::pi;
    }

    float indexDirection = linesAngle + juce::MathConstants<float>::halfPi;



    float startR = floor(rOuter * 2.0 / lineSpacing) * lineSpacing / 2.0;

    float startX = centerX - startR * cos(indexDirection);

    float startY = centerY - startR * sin(indexDirection);

    for (float i = 0; i < rOuter * 2.0; i += lineSpacing) {

        std::vector<float> intersectionPoints = findCircleLineIntersections(startX + i * cos(indexDirection), startY + i * sin(indexDirection), startX + i * cos(indexDirection) + 10.0 * cos(linesAngle), startY + i * sin(indexDirection) + 10.0 * sin(linesAngle), centerX, centerY, rInner, rOuter);

        for (int j = 0; j < intersectionPoints.size(); j += 4)
        {
            juce::Line<float> line;
            line.setStart(intersectionPoints[j], intersectionPoints[j+1]);
            line.setEnd(intersectionPoints[j + 2], intersectionPoints[j + 3]);
            g.drawLine(line);

        }
    }
}


std::vector<float> NonsenseLookAndFeel::findCircleLineIntersections(float lineX1, float lineY1, float lineX2, float lineY2, float circleCenterX, float circleCenterY, float circleInnerR, float circleOuterR)
{
    std::vector<float> intersections = std::vector<float>();

    float x1 = lineX1 - circleCenterX;
    float y1 = lineY1 - circleCenterY;

    float x2 = lineX2 - circleCenterX;
    float y2 = lineY2 - circleCenterY;

    float dx = x2 - x1;
    float dy = y2 - y1;
    float dr = sqrt(dx * dx + dy * dy);

    float D = x1 * y2 - x2 * y1;

    if (circleOuterR * circleOuterR * dr * dr - D * D <= 0) {
        return intersections;
    }

    float oiax = (D * dy + (dy >= 0) * dx * sqrt(circleOuterR * circleOuterR * dr * dr - D * D)) / (dr * dr) + circleCenterX;
    float oiay = (-D * dx + abs(dy) * sqrt(circleOuterR * circleOuterR * dr * dr - D * D)) / (dr * dr) + circleCenterY;

    intersections.push_back(oiax);
    intersections.push_back(oiay);

    if (circleInnerR * circleInnerR * dr * dr - D * D > 0)
    {
        float iiax = (D * dy + (dy >= 0) * dx * sqrt(circleInnerR * circleInnerR * dr * dr - D * D)) / (dr * dr) + circleCenterX;
        float iiay = (-D * dx + abs(dy) * sqrt(circleInnerR * circleInnerR * dr * dr - D * D)) / (dr * dr) + circleCenterY;

        float iibx = (D * dy - (dy >= 0) * dx * sqrt(circleInnerR * circleInnerR * dr * dr - D * D)) / (dr * dr) + circleCenterX;
        float iiby = (-D * dx - abs(dy) * sqrt(circleInnerR * circleInnerR * dr * dr - D * D)) / (dr * dr) + circleCenterY;

        intersections.push_back(iiax);
        intersections.push_back(iiay);
        intersections.push_back(iibx);
        intersections.push_back(iiby);
    }

    float oibx = (D * dy - (dy >= 0) * dx * sqrt(circleOuterR * circleOuterR * dr * dr - D * D)) / (dr * dr) + circleCenterX;
    float oiby = (-D * dx - abs(dy) * sqrt(circleOuterR * circleOuterR * dr * dr - D * D)) / (dr * dr) + circleCenterY;

    intersections.push_back(oibx);
    intersections.push_back(oiby);

    return intersections;
}