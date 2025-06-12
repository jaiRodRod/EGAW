/*
  ==============================================================================

    BidirectionalLinkedViewport.h
    Created: 2 Apr 2025 6:40:14pm
    Author:  jaezro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class BidirectionalLinkedViewport : public juce::Viewport {
public:

	void setVerticalLinkedViewport(BidirectionalLinkedViewport* verticalLink) {
		verticalLinkedViewport = verticalLink;
	};

	void setHorizontalLinkedViewport(BidirectionalLinkedViewport* horizontalLink) {
		horizontalLinkedViewport = horizontalLink;
	};

    void setLinkedViewports(BidirectionalLinkedViewport* verticalLink,
        BidirectionalLinkedViewport* horizontalLink) {
        verticalLinkedViewport = verticalLink;
        horizontalLinkedViewport = horizontalLink;
    };

    void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea) override {

        getVerticalScrollBar().toFront(false);
        getHorizontalScrollBar().toFront(false);

        if (!isUpdating) {
            juce::ScopedValueSetter<bool> guard(isUpdating, true);

            if (verticalLinkedViewport) {
                verticalLinkedViewport->setViewPosition(
                    verticalLinkedViewport->getViewPositionX(),
                    newVisibleArea.getY()
                );
            }

            if (horizontalLinkedViewport) {
                horizontalLinkedViewport->setViewPosition(
                    newVisibleArea.getX(),
                    horizontalLinkedViewport->getViewPositionY()
                );
            }
        }
    };

private:
    BidirectionalLinkedViewport* verticalLinkedViewport = nullptr;
    BidirectionalLinkedViewport* horizontalLinkedViewport = nullptr;
    bool isUpdating = false;
};
