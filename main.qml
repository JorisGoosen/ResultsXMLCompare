import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 1.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Window {
	visible:	true
	width:		640
	height:		480
	title:		qsTr("JASP Results compare tool")

	Rectangle
	{
		anchors.fill:	parent
		color:			black

		SplitView
		{
			id:				splitVertical
			orientation:	Qt.Vertical

			anchors.fill: parent

			SplitView
			{
				id:				splitHorizontal
				orientation:	Qt.Horizontal
				height:			splitVertical.height / 2

				ScrollView
				{
					clip:		true
					width:		splitHorizontal.width / 2

					TextEdit
					{
						id:		links
						text:	"<node>example links!</node>"
						color:	yellow
					}
				}

				ScrollView
				{
					clip:		true
					width:		splitHorizontal.width / 2

					TextEdit
					{
						id:		rechts
						text:	"<node>example rechts!</node>"
						color:	yellow

					}
				}
			}

			Item
			{
				Layout.fillHeight:	true

				Button
				{
					id:		compareButton
					text:	"Compare"
					anchors
					{
						left:	parent.left
						right:	parent.right
					}

					onClicked:
					{
						output.text = compareResults.compare(links.text, rechts.text)
					}
				}

				ScrollView
				{
					clip: true

					anchors
					{
						top:	compareButton.bottom
						left:	parent.left
						right:	parent.right
						bottom:	parent.bottom
					}

					TextEdit
					{
						id:					output
						text:				"Something something something...."
						color:				yellow
					}
				}
			}
		}
	}
}
