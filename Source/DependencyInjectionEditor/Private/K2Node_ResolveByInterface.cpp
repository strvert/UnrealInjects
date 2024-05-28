#include "K2Node_ResolveByInterface.h"

#define LOCTEXT_NAMESPACE "DependencyInjection"

namespace K2Node_ResolveByInterfacePins
{
	static const FName ClassPinName("Class");
}

void UK2Node_ResolveByInterface::AllocateDefaultPins()
{
	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	// If required add the world context pin
	// if (UseWorldContext())
	// {
	// 	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), FK2Node_ConstructObjectFromClassHelper::GetWorldContextPinName());
	// }

	// Add blueprint pin
	UEdGraphPin* ClassPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Class, GetClassPinBaseClass(), K2Node_ResolveByInterfacePins::ClassPinName);
	
	// Result pin
	UEdGraphPin* ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, GetClassPinBaseClass(), UEdGraphSchema_K2::PN_ReturnValue);
	
	// if (UseOuter())
	// {
	// 	UEdGraphPin* OuterPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), UEdGraphSchema_K2::PN_Self);
	// 	OuterPin->PinFriendlyName = FText::FromName(FK2Node_ConstructObjectFromClassHelper::GetOuterPinFriendlyName());
	// }
}

FText UK2Node_ResolveByInterface::GetBaseNodeTitle() const
{
	return LOCTEXT("ResolveByInterface_BaseTitle", "Resolve By Interface");
}

FText UK2Node_ResolveByInterface::GetNodeTitleFormat() const
{
	return LOCTEXT("ResolveByInterface", "Resolve Object By Interface {ClassName}");
}


#undef LOCTEXT_NAMESPACE
