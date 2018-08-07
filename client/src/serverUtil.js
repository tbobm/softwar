export const parseResponse = (response) => {
  const res = response.split('|');
  return { status: res[0], data: res[1] };
};

export const parseCommand = (command, argument = null) => `${command}|${argument}`;

export const getRandomInt = (max) => {
  return Math.floor(Math.random() * Math.floor(max));
}