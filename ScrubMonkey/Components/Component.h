#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <vector>
#include <string>

/*!
 * @brief Base class for components.
 */
class Component
{
public:
	
	/*!
	 * @brief Constructs a component with a name
	 */
	Component(std::string name, int driverStationDigitalIn);
	
	/*!
	 * @brief Frees the resources allocated by the component.
	 */
	virtual ~Component();

	/*!
	 * @brief Creates all components.
	 */
	static void CreateComponents();
	
	/*!
	 * @brief Destroys all components.
	 */
	static void DestroyComponents();
	
	/*!
	 * @brief Calls Update() on all created components
	 */
	static void UpdateAll();
	
	/*!
	 * @brief Updates the component each cycle.
	 */
	void Update();
	
	/*!
	 * @brief Returns whether or not the robot requires to be in an enabled state to run the enabled function.
	 * @return whether the component requires an enabled state
	 */
	bool EnableRequired();
	
	/*!
	 * @brief Gets the driver station digital input channel for the component.
	 * @return the digital input channel
	 */
	int GetDigitalIn();

	/*!
	 * @brief Gets the component's name.
	 * @return the name of the component
	 */
	std::string GetName();
	
protected:
	/*!
	 * @brief Allows the component to update itself when enabled.
	 */
	virtual void UpdateEnabled() = 0;
	
	/*!
	 * @brief Allows the component to execute code when disabled.
	 */
	virtual void UpdateDisabled() = 0;
	
	/*!
	 * @brief Called the first cycle that the component is enabled.
	 */
	virtual void OnEnabled() = 0;
	
	/*!
	 * @brief Called the first cycle that the component is disabled.
	 */
	virtual void OnDisabled() = 0;
	
private:
	static std::vector<Component*> component_vector;
	
	std::string m_name;
	int m_digitalIn;
	bool m_lastEnabled;
};

#endif //COMPONENT_H_
